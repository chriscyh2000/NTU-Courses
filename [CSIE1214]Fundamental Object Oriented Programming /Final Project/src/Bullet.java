import javax.swing.*;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.util.ArrayList;

public class Bullet extends Weapon{
	private int x, y;
	public ArrayList<Brick> brickList;
	public Bullet(Player myUser, ArrayList<Brick> bList) {
		super(myUser);
		brickList = bList;
		super.count = 1;
	}
	
	@Override
	public boolean perform(Player enemy) {
		if(new Rectangle(getX(), getY(), 8, 8)
				.intersects(new Rectangle(enemy.X, enemy.Y, 50, 50))) {
			user.score += 10;
			enemy.lives -= 1;
			user.Shoot = false;
			user.shootDirection = "";
			user.setUsingWeapon(null);
			return true;
		}
		for(Brick br : brickList)
			if(br.collisionCheck(1, getX(), getY(), 8, 8)){
			user.Shoot = false;
			user.shootDirection = "";
			user.setUsingWeapon(null);
			return true;
		}
		if(getY() < 1 || getY() > 580 || getX() < 1 || getX() > 640){
			user.Shoot = false;
			user.shootDirection = "";
			user.setUsingWeapon(null);
			return true;
		}
		return false;
	}
	
	public void setX(int X){
		x = X;
	}
	public void setY(int Y){
		y = Y;
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
	public void move(String face) {
		if(face.equals("right"))
			x += 3;
		else if(face.equals("left"))
			x -= 3;
		else if(face.equals("up"))
			y -= 3;
		else
			y += 3;
	}

	@Override
	public void draw(Component c, Graphics g) {
		ImageIcon bulletIMG = new ImageIcon("enemy_bullet.png");
		bulletIMG.paintIcon(c, g, x, y);
	}

	@Override
	public Weapon create(Player user) {
		Bullet newBullet = new Bullet(user, brickList);
		newBullet.setX(user.X);
		newBullet.setY(user.Y);
		if(user.up || user.down) newBullet.setX(user.X + 22);
		if(user.down) newBullet.setY(user.Y + 50);
		if(user.left || user.right) newBullet.setY(user.Y + 22);
		if(user.right) newBullet.setX(user.X + 50);
		return newBullet;
	}
	
	@Override
	public Weapon releaseNewOne(){
		return create(user);
	}

	
}
