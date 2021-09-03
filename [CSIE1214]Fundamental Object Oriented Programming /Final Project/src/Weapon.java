import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.ImageIcon;

public class Weapon {
    public Weapon(Player myUser){
        user = myUser;
    }
    public int count;
    public Player user;
    public boolean perform(Player enemy) {return false;}
    public void draw(Component c,Graphics g) {return;}
    public int getX() {return 0;}
    public int getY() {return 0;}
    public void move(String face) {return;}
    public Weapon create(Player myUser) {return null;}
    public Weapon releaseNewOne(){return null;}
}
