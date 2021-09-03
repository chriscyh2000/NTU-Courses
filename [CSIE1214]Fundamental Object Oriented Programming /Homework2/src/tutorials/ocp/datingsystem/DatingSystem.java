package tutorials.ocp.datingsystem;

import tutorials.ocp.datingsystem.User.Gender;
import tutorials.ocp.datingsystem.composite.CompositeMatchPolicy;
import tutorials.ocp.datingsystem.decorator.FemaleOnly;
import tutorials.ocp.datingsystem.decorator.HigherSalary;
import tutorials.ocp.datingsystem.decorator.LongText;

/**
 * This is a Dating-System that every user can be matched to certain users based on some match-policies.
 * <p>
 * How do you support different match-policy following OCP?
 *
 * @author - johnny850807@gmail.com (Waterball)
 */
public class DatingSystem {

    public static void main(String[] args) {
        // initialization procedure begins
        User me = new User(Gender.MALE, "Waterball", "FOOP TA", 10);

        // An intuitive way
        me.setMatchPolicy(new FemaleOnlyPolicy());  // dependency-injection via its setter

        // or
        me.setMatchPolicy(new HigherSalaryPolicy());

        // or
        me.setMatchPolicy(new LongTextPolicy());

        // but, can you support different combinations of the policies?
        // Well, let's do the following
        // (1)
        me.setMatchPolicy(new CompositeMatchPolicy(new FemaleOnlyPolicy(), new HigherSalaryPolicy(), new LongTextPolicy()));
        // or (2)
        me.setMatchPolicy(new FemaleOnly(new HigherSalary(new LongText())));

        // we can say this follows OCP, since we can add the new policy: NewPolicy, simply by:
        me.setMatchPolicy(new CompositeMatchPolicy(new FemaleOnlyPolicy(), new HigherSalaryPolicy(), new LongTextPolicy(),
                /* Here we are, we can put the new policy here,
                without any modifications to the old code except the initialization-procedure */
                new NewPolicy()));

        // Then, start the matching
        User[] candidates = {
                new User(Gender.MALE, "A", "A", 40000000),
                new User(Gender.FEMALE, "B", "A", 100),
                new User(Gender.FEMALE, "C", "HA... I'm a very extrovert girl", 0),
                new User(Gender.FEMALE, "Best", "HA... I'm a very extrovert girl", 300000),
        };
        me.match(candidates);
        for (User friend : me.getFriends()) {
            System.out.println(friend.getName());  // output: Best
        }

        // You may wonder, can I directly inject an array or a list to support several policies?
        // e.g., `me.setMatchPolicies(new MatchPolicy[] {policy1, policy2, policy3, ...})`
        // Yes, you can! This also follows OCP, just two drawbacks:
        // (1) The encapsulation of 'Match-Policy' is bad (A great encapsulation -> a well defined interface and self-contained responsibility)
        // (2) It doesn't follow "Dependency-Inversion principle (DIP)". (Google it! Be patient, it's a long journey :D)
    }
}

class NewPolicy implements MatchPolicy {
    @Override
    public boolean match(User user, User candidate) {
        return true; // support some new policy ...
    }
}
