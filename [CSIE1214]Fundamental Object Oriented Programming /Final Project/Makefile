JFLAGS = -g
JC = javac

.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) -cp src/ $*.java

CLASSES = \
	./src/Brick.java \
	./src/Solid_Bricks.java \
	./src/Breakable_Bricks.java \
	./src/Player.java \
	./src/Weapon.java \
	./src/Bullet.java \
	./src/Missile.java \
	./src/Bomb.java \
	./src/Buff.java \
	./src/Health.java \
	./src/Magazine.java \
	./src/BuffGenerator.java \
	./src/Gameplay.java \
	./src/Main.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class

run:
	java -cp src/ Main