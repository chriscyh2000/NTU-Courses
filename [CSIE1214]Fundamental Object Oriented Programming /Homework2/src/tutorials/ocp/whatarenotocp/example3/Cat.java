package tutorials.ocp.whatarenotocp.example3;

/**
 * @author Waterball (johnny850807@gmail.com)
 */
public class Cat {
    private final static int NORMAL = 0;
    private final static int SAD = 1;
    private int state = NORMAL;

    public void makeAction1() {
        // Violate OCP: coupled with the concrete state "SAD"
        if (state != SAD) {
            // make an action
        }
    }

    public void makeAction2() {
        // !! Also violate OCP: you are still coupling with the concrete state "1" (even you don't name it explicitly!)
        if (state != 1) {
            // make an action
        }
    }
}


