package tutorials.ocp.whatarenotocp.example2;

/**
 * @author Waterball (johnny850807@gmail.com)
 */
public class Cat {
    // Violate OCP: directly coupled with the action eat
    public void eat() {
        // eating
    }

    // Violate OCP: directly coupled with the action sleep
    public void sleep() {
        // sleeping
    }
}
