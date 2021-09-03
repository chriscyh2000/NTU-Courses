import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.ImageIcon;

public class Brick {
    public int brickXpos[];
    public int brickYpos[];
    public boolean brickOn[];
    public int brickWidth;
    public int brickHeight;
    public ImageIcon brickImage;
    public int count[];
    public Brick(){
        brickWidth = 50;
        brickHeight = 50;
    }
    public void draw(Component component, Graphics graphics){
        for(int i = 0; i < brickOn.length; ++i) {
            if(brickOn[i])
                brickImage.paintIcon(component, graphics, brickXpos[i], brickYpos[i]);
        }
    }
    public boolean collisionCheck(int damage, int x, int y, int width, int height){
        return true;
    }
}
