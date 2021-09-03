package tutorials;

import tw.waterball.foop.hw2.provided.OnePunch;
import tw.waterball.foop.hw2.provided.Target;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class HowToUseOnePunch {

    public static void main(String[] args) {
        final OnePunch onePunch = new OnePunch();
        final Unit unit = new Unit();

        System.out.printf("MP's cost: %d\n", onePunch.getMpCost());

        // Use onePunch targeting the unit.
        onePunch.perform(unit);

        // The outcome will be the same every time you run the main method
        // as OnePunch uses a fixed pseudo-random implementation to calculate its damage.
    }
}

// Note: only the class that implements the Target interface can be passed into the onePunch's perform method.
class Unit implements Target {
    @Override
    public void takeOnePunchDamage(int damage) {
        System.out.printf("The unit is damaged %d.\n", damage);
    }
}