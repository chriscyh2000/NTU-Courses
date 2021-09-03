package tutorials.ocp.datingsystem.composite;

import tutorials.ocp.datingsystem.MatchPolicy;
import tutorials.ocp.datingsystem.User;

import java.util.Arrays;
import java.util.List;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class CompositeMatchPolicy implements MatchPolicy {
    private List<MatchPolicy> matchPolicies;

    public CompositeMatchPolicy(MatchPolicy ...matchPolicies) {
        this.matchPolicies = Arrays.asList(matchPolicies);
    }

    @Override
    public boolean match(User user, User candidate) {
        for (MatchPolicy matchPolicy : matchPolicies) {
            if (!matchPolicy.match(user, candidate)) {
                return false;
            }
        }
        return true;
    }
}
