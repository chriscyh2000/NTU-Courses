import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;

public class Player {
    private ArrayList<Weapon> WeaponList;
    private Player enemy;
    private Weapon usingWeapon;
    private ImageIcon playerIMG;
    public int EffectCount;
    public int Speed;
    public int X;
    public int Y;
    public boolean right;
    public boolean left;
    public boolean down;
    public boolean up;
    public int score = 0;
    public int lives = 5;
    public boolean Shoot;
    public String shootDirection = "";
    private String right_img;
    private String left_img;
    private String up_img;
    private String down_img;
    public ArrayList<Brick> BrickList;
    public Listener myListener;
    private int command1; //up
    private int command2; //down
    private int command3; //left
    private int command4; //right
    private int command5; //bullet
    private int command6; //missile
    private int command7; //bomb
    public Player(){};
    public Player(int x, int y, boolean r, boolean l, boolean d, boolean u, String RightIMG, String LeftIMG, String UpIMG, String DownIMG,
                  int c1, int c2, int c3, int c4, int c5, int c6, int c7, ArrayList<Weapon> wList){
        myListener = new Listener();
        X = x;
        Y = y;
        right = r;
        left = l;
        down = d;
        up = u;
        right_img = RightIMG;
        left_img = LeftIMG;
        up_img = UpIMG;
        down_img = DownIMG;
        BrickList = new ArrayList<>();
        WeaponList = new ArrayList<>();
        if(u)
            playerIMG = new ImageIcon(UpIMG);
        else if(d)
            playerIMG = new ImageIcon(DownIMG);
        else if(r)
            playerIMG = new ImageIcon(RightIMG);
        else playerIMG = new ImageIcon(LeftIMG);
        for(int i = 0; i < wList.size(); ++i){
            Weapon w = wList.get(i);
            WeaponList.add(w.create(this));
        }
        command1 = c1;
        command2 = c2;
        command3 = c3;
        command4 = c4;
        command5 = c5;
        command6 = c6;
        command7 = c7;
        usingWeapon = null;
        Speed = 10;
    }
    public ImageIcon getPlayerIMG(){
        return playerIMG;
    }
    public Weapon getUsingWeapon(){return usingWeapon;}

    public void draw(Component component, Graphics graphics){
        playerIMG.paintIcon(component, graphics, X, Y);
    }

    public void setEnemy(Player Enemy){
        enemy = Enemy;
    }

    public ArrayList<Weapon> getWeaponList(){
        return WeaponList;
    }
    public void setUsingWeapon(Weapon w){
        usingWeapon = w;
    }
    private class Listener implements KeyListener{
        public void keyTyped(KeyEvent e) {}
        public void keyReleased(KeyEvent e) {}
        public void keyPressed(KeyEvent e) {
            if(e.getKeyCode()== command1){
                playerIMG = new ImageIcon(up_img);
                right = false;
                left = false;
                down = false;
                up = true;
                boolean testCollision = false;
                if(Y >= 10){
                    for(int i = 0; i < BrickList.size(); ++i){
                        Brick b = BrickList.get(i);
                        if(testCollision) break;
                        for(int j = 0; j < b.brickOn.length; ++j){
                            if (new Rectangle(X, Y - Speed, 50, 50).intersects(new Rectangle(b.brickXpos[j], b.brickYpos[j], b.brickWidth, b.brickHeight))
                                    && b.brickOn[j]) {
                                testCollision = true;
                                break;
                            }
                        }
                    }
                    if(new Rectangle(X , Y - Speed, 50, 50).intersects(new Rectangle(enemy.X, enemy.Y, 50, 50)))
                        testCollision = true;
                    if(!testCollision){
                        Y -= Speed;
                    }
                }
            }
            if(e.getKeyCode()== command3){
                playerIMG = new ImageIcon(left_img);
                right = false;
                left = true;
                down = false;
                up = false;
                boolean testCollision = false;
                if(X >= 10){
                    for(int i = 0; i < BrickList.size(); ++i){
                        Brick b = BrickList.get(i);
                        if(testCollision) break;
                        for(int j = 0; j < b.brickOn.length; ++j){
                            if (new Rectangle(X - Speed, Y, 50, 50).intersects(new Rectangle(b.brickXpos[j], b.brickYpos[j], b.brickWidth, b.brickHeight))
                                    && b.brickOn[j]) {
                                testCollision = true;
                                break;
                            }
                        }
                    }
                    if(new Rectangle(X - Speed, Y, 50, 50).intersects(new Rectangle(enemy.X, enemy.Y, 50, 50)))
                        testCollision = true;
                    if(!testCollision) X -= Speed;
                }
            }
            if(e.getKeyCode()== command2){
                playerIMG = new ImageIcon(down_img);
                right = false;
                left = false;
                down = true;
                up = false;
                boolean testCollision = false;
                if(Y <= 540){
                    for(int i = 0; i < BrickList.size(); ++i){
                        Brick b = BrickList.get(i);
                        if(testCollision) break;
                        for(int j = 0; j < b.brickOn.length; ++j){
                            if (new Rectangle(X, Y + Speed, 50, 50).intersects(new Rectangle(b.brickXpos[j], b.brickYpos[j], b.brickWidth, b.brickHeight))
                                    && b.brickOn[j]) {
                                testCollision = true;
                                break;
                            }
                        }
                    }
                    if(new Rectangle(X , Y + Speed, 50, 50).intersects(new Rectangle(enemy.X, enemy.Y, 50, 50)))
                        testCollision = true;
                    if(!testCollision) Y += Speed;
                }
            }
            if(e.getKeyCode() == command4){
                playerIMG = new ImageIcon(right_img);
                right = true;
                left = false;
                down = false;
                up = false;
                boolean testCollision = false;
                if(X <= 595){
                    for(int i = 0; i < BrickList.size(); ++i){
                        Brick b = BrickList.get(i);
                        if(testCollision) break;
                        for(int j = 0; j < b.brickOn.length; ++j){
                            if(new Rectangle(X + Speed, Y, 50, 50).intersects(new Rectangle(b.brickXpos[j], b.brickYpos[j], b.brickWidth, b.brickHeight))
                                    && b.brickOn[j]) {
                                testCollision = true;
                                break;
                            }
                        }
                    }
                    if(new Rectangle(X + Speed, Y, 50, 50).intersects(new Rectangle(enemy.X, enemy.Y, 50, 50)))
                        testCollision = true;
                    if(!testCollision) X += Speed;
                }
            }
            if(e.getKeyCode() == command5){
                if(Shoot) return;
                Shoot = true;
                usingWeapon = WeaponList.get(0).releaseNewOne();
            }
            if(e.getKeyCode() == command6){
                if(Shoot) return;
                Shoot = true;
                usingWeapon = WeaponList.get(1).releaseNewOne();
            }
            if(e.getKeyCode() == command7){
                if(Shoot) return;
                Shoot = true;
                usingWeapon = WeaponList.get(2).releaseNewOne();
            }
        }
    }
}
