import javax.swing.*;
import java.awt.*;
import java.util.*;

public class Buff {
    public int buffXpos;
    public int buffYpos;
    public boolean buffOn;
    public int buffWidth;
    public int buffHeight;
    Buff(){
        buffWidth = 30;
        buffHeight = 30;
    }
    public boolean acquireCheck(Player p){
        return false;
    }
    public boolean isCollision(ArrayList<Brick>brickList){
        for(Brick br : brickList){
            if(br.collisionCheck(0, buffXpos, buffYpos, buffWidth, buffHeight)){
                return false; // if collision with bricks
            }
        }
        return true;
    }
    public void generateBuff(ArrayList<Brick>brickList){
        while(true){
            //random position as 10 units increment
            buffXpos = (int)(10 * Math.floor(Math.random() * 61));
            buffYpos = (int)(10 * Math.floor(Math.random() * 56));
            if(isCollision(brickList)){
                break;
            }
        }
        return;
    }
    public void draw(Component component, Graphics graphics){}
}

