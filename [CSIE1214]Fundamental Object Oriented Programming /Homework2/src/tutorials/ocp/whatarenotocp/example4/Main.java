package tutorials.ocp.whatarenotocp.example4;

/**
 * @author Waterball (johnny850807@gmail.com)
 */
public class Main {
    public static Action[] actions = {/*some actions*/};


    public static void main(String[] args) {

    }

    // Violate OCP -> Never put your interface in the entry class (Main)
    interface Action {
        void execute(Cat cat);
    }
}
