import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

public class Magazine extends Buff{
    public ImageIcon MagazineImage;
    public Magazine(int x, int y) {
        MagazineImage = new ImageIcon("magazine.png");
        buffXpos = x;
        buffYpos = y;
        buffOn =true;
        buffWidth = 30;
        buffHeight = 30;
    }
    public void draw(Component component, Graphics graphics){

        if(buffOn)
            MagazineImage.paintIcon(component, graphics, buffXpos, buffYpos);

    }

    public boolean acquireCheck(Player p){
        if(new Rectangle(buffXpos, buffYpos, 30, 30)
                .intersects(new Rectangle(p.X, p.Y, 50, 50))) {
            p.getWeaponList().get(1).count+=10;
            p.getWeaponList().get(2).count+=10;
            buffOn = false;
            return true;
        }
        return false;
    }
}
