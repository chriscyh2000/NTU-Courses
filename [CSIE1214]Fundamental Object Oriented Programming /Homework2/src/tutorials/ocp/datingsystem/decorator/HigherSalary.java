package tutorials.ocp.datingsystem.decorator;

import tutorials.ocp.datingsystem.MatchPolicy;
import tutorials.ocp.datingsystem.User;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class HigherSalary extends AbstractMatchPolicy {
    public HigherSalary() {
    }

    public HigherSalary(MatchPolicy next) {
        super(next);
    }

    @Override
    public boolean match(User user, User candidate) {
        return candidate.getSalary() >= user.getSalary() && next.match(user, candidate);
    }
}
