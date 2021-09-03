package tutorials;

import tw.waterball.foop.hw2.provided.AI;

import java.util.Arrays;
import java.util.List;

import static java.util.Arrays.asList;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class HowToUseAI {

    public static void main(String[] args) {
        final AI ai = new AI();

        // Given you have 3 skills, the 0th action is "basic-attack"
        String[] actions = {"basic attack", "waterball", "fireball", "others"};
        int action = ai.selectAction(asList(0, 1, 2, 3));
        System.out.printf("Use action: %s.", actions[action]);

        // if you are targeting 3 units from a list of 10 unit candidates, you should write:
        List<Integer> targets = ai.selectTarget(10, 3);
        for (int i = 0; i < targets.size(); i++) {
            System.out.printf("Target %d: %d\n", i, targets.get(i));
        }

        // The outcome will be the same every time you run the main method,
        // as the AI uses a fixed pseudo-random implementation to make his decisions.
    }
}