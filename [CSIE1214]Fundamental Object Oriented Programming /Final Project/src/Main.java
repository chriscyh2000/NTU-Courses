import javax.swing.*;
import java.awt.*;

public class Main{
    public static void main(String[] args) {
        JFrame obj=new JFrame();
        Gameplay gamePlay = new Gameplay();

        obj.setBounds(10, 10, 800, 630);
        obj.setTitle("Tank 2D Game!");
        obj.setBackground(Color.gray);
        obj.setResizable(false);

        obj.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        obj.add(gamePlay);
        obj.setVisible(true);
    }
}