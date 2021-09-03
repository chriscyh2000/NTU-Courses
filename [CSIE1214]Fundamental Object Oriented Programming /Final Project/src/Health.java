import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

public class Health extends Buff{
    public ImageIcon HealthImage;
    public Health(int x, int y) {
        super();
        HealthImage = new ImageIcon("health.png");
        buffXpos = x;
        buffYpos = y;
        buffOn = true;
        buffWidth = 30;
        buffHeight = 30;
    }
    public void draw(Component component, Graphics graphics){
        if(buffOn)
            HealthImage.paintIcon(component, graphics, buffXpos, buffYpos);
    }
    @Override
    public boolean acquireCheck(Player p){
        if(new Rectangle(buffXpos, buffYpos, 30, 30)
                .intersects(new Rectangle(p.X, p.Y, 50, 50))) {
            p.lives += 1;
            buffOn = false;
            return true;
        }
        return false;
    }
}
