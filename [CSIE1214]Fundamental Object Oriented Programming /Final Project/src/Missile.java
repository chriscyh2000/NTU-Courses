import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Queue;

public class Missile extends Weapon{
    private int x, y;
    public ArrayList<Brick> brickList;
    public ImageIcon missileImage;
    public ArrayList<ExplosionEffect> effect_list;
    private boolean up;
    private boolean down;
    private boolean left;
    private boolean right;
    public Missile(Player myUser, ArrayList<Brick> bList) {
        super(myUser);
        super.count = 10;
        brickList = bList;
        if(user.up) missileImage = new ImageIcon("missile_up.png");
        else if(user.down) missileImage = new ImageIcon("missile_down.png");
        else if(user.left) missileImage = new ImageIcon("missile_left.png");
        else  missileImage = new ImageIcon("missile_right.png");
        effect_list = new ArrayList<>();
        up = user.up;
        down = user.down;
        right = user.right;
        left = user.left;
    }
    @Override
    public boolean perform(Player enemy) {
        if(user.shootDirection.equals("hit") && effect_list.isEmpty()) {
            user.shootDirection = "";
            user.Shoot = false;
            return true;
        }
        else if(user.shootDirection.equals("hit") && !effect_list.isEmpty()) return false;
        if(left || right){
            if(new Rectangle(getX(), getY(), 50, 24)
                    .intersects(new Rectangle(enemy.X, enemy.Y, 50, 50))) {
                user.score += 10;
                enemy.lives -= 1;
                user.shootDirection = "hit";
                ExplosionEffect e = new ExplosionEffect(enemy.X+13,enemy.Y);
                e.countDown = 10;
                e.limit = 5;
                effect_list.add(e);
                return false;
            }
        }
        else if(up || down){
            if(new Rectangle(getX(), getY(), 24, 50)
                    .intersects(new Rectangle(enemy.X, enemy.Y, 50, 50))) {
                user.score += 10;
                enemy.lives -= 1;
                user.shootDirection = "hit";
                ExplosionEffect e = new ExplosionEffect(enemy.X, enemy.Y + 13);
                e.countDown = 10;
                e.limit = 5;
                effect_list.add(e);
                return false;
            }
        }
        for(int i = 0; i < brickList.size(); ++i){
            Brick br = brickList.get(i);
            if(up || down){
                if(br.collisionCheck(2, getX(), getY(), 24, 50)) {
                    if(i == 0){
                        user.Shoot = false;
                        user.shootDirection = "";
                        return true;
                    }
                    else{
                        int Y;
                        if(up) Y = y - 30 ;
                        else Y = y + 50;
                        ExplosionEffect e = new ExplosionEffect(x, Y);
                        effect_list.add(e);
                        return false;
                    }
                }
            }
            else if(right || left){
                if(br.collisionCheck(2, getX(), getY(), 50, 24)) {
                    if(i == 0){
                        user.Shoot = false;
                        user.shootDirection = "";
                        return true;
                    }
                    else{
                        int X;
                        if(left) X = x - 50;
                        else X = x + 50;
                        ExplosionEffect e = new ExplosionEffect(X, y-13);
                        effect_list.add(e);
                        return false;
                    }
                }
            }
        }
        if(getY() < 1 || getY() > 580 || getX() < 1 || getX() > 615){
            ExplosionEffect e = new ExplosionEffect(enemy.X + 15, enemy.Y + 15);
            user.Shoot = false;
            user.shootDirection = "";
            return true;
        }
        return false;
    }
    @Override
    public void draw(Component c, Graphics g) {
        for(ExplosionEffect e : effect_list){
            e.effect_paint(c, g);
            if(e.countDown <= 0) effect_list.remove(e);
        }
        if(user.Shoot) missileImage.paintIcon(c, g, x, y);
    }
    @Override
    public int getX() {
        return x;
    }
    @Override
    public int getY() {
        return y;
    }
    @Override
    public void move(String face){
        if(user.shootDirection.equals("hit")) return;
        if (face.equals("right")){
            x += 2;
        }
        else if (face.equals("left")){
            x -= 2;
        }
        else if (face.equals("up")){
            y -= 2;
        }
        else {
            y += 2;
        }
    }
    public void setX(int X){
        x = X;
    }
    public void setY(int Y){
        y = Y;
    }
    @Override
    public Weapon create(Player user) {
        Missile newMissile = new Missile(user, brickList);
        newMissile.setX(user.X);
        newMissile.setY(user.Y);
        if(user.up || user.down) newMissile.setX(user.X + 14);
        if(user.down) newMissile.setY(user.Y + 50);
        if(user.left || user.right) newMissile.setY(user.Y + 14);
        if(user.right) newMissile.setX(user.X + 50);
        return newMissile;
    }
    @Override
    public Weapon releaseNewOne(){
        if (count > 0) {
            --count;
            return  this.create(user);
        }
        else {
            user.Shoot = false;
            user.shootDirection = "";
            return null;
        }
    }
    public class ExplosionEffect{
        public ImageIcon effectIMG1;
        public ImageIcon effectIMG2;
        public int countDown;
        public int limit;
        public int X;
        public int Y;
        public ExplosionEffect(int x, int y){
            effectIMG1 = new ImageIcon("explosion_effect1.png");
            effectIMG2 = new ImageIcon("explosion_effect2.png");
            countDown = 34;
            limit = 17;
            X = x;
            Y = y;
        }
        public void effect_paint(Component c, Graphics g){
            --countDown;
            if(countDown > limit)
                effectIMG1.paintIcon(c, g, X-5, Y-5);
            else effectIMG2.paintIcon(c, g, X-5, Y-5);
        }
    }
}