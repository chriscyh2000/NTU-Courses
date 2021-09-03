import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

public class Breakable_Bricks extends Brick{
    public ImageIcon broken;

    public Breakable_Bricks(){
        super();
        broken = new ImageIcon("break_brick_cracked.jpg");
        brickImage = new ImageIcon("break_brick.jpg");
        int x[] = {50,350,450,550,50,300,350,450,550,150,150,450,550,
                   250,50,100,150,550,250,350,450,550,50,250,350,550,
                   50,150,250,300,350,550,50,150,250,350,450,550,50,
                   250,350,550};
        brickXpos = x;
        int y[] = {50,50,50,50,100,100,100,100,100,150,200,200,200,250,
                   300,300,300,300,350,350,350,350,400,400,400,400,450,
                   450,450,450,450,450,500,500,500,500,500,500,550,550,
                   550,550};
        brickYpos = y;
        count = new int[42];
        brickOn = new boolean[42];
        Arrays.fill(count, 2);
        Arrays.fill(brickOn, true);
    }
    @Override
    public boolean collisionCheck(int damage, int x, int y, int width, int height){
        boolean collided = false;
        for(int i=0; i< brickOn.length; ++i) {
            if(brickOn[i]){
                if(new Rectangle(x, y, width, height).intersects(new Rectangle(brickXpos[i], brickYpos[i], brickWidth, brickHeight))) {
                    count[i] -= damage;
                    if(count[i] <= 0)
                        brickOn[i] = false;
                    collided = true;
                }
            }
        }
        return collided;
    }
    @Override
    public void draw(Component component, Graphics graphics){
        for(int i = 0; i < brickOn.length; ++i) {
            if(brickOn[i]){
                if(count[i] > 1) brickImage.paintIcon(component, graphics, brickXpos[i], brickYpos[i]);
                else broken.paintIcon(component, graphics, brickXpos[i], brickYpos[i]);
            }
        }
    }
}
