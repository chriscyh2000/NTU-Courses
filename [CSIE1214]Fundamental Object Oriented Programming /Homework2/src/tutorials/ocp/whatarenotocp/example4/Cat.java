package tutorials.ocp.whatarenotocp.example4;

/**
 * @author Waterball (johnny850807@gmail.com)
 */
public class Cat {

    public void executeAllActions() {
        // Violate OCP! Do not couple your module with the entry class (Main)
        // The direct coupling to static fields is always a bad design (Because the dependency flow is implicit)
        Main.Action[] actions = Main.actions;
        for (Main.Action action : actions) {
            action.execute(this);
        }
    }
}

// Instead, you should do "dependency-injection" as follows:
class GoodCat {
    private Action[] actions;

    // dependency-injection via the constructor --> BETTER
    public GoodCat(Action... actions) {
        this.actions = actions;
    }

    interface Action {
        void execute(Cat cat);
    }
}
