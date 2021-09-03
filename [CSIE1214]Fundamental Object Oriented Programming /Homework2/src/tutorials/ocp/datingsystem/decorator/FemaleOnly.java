package tutorials.ocp.datingsystem.decorator;

import tutorials.ocp.datingsystem.MatchPolicy;
import tutorials.ocp.datingsystem.User;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class FemaleOnly extends AbstractMatchPolicy {
    public FemaleOnly() {
    }

    public FemaleOnly(MatchPolicy next) {
        super(next);
    }

    @Override
    public boolean match(User user, User candidate) {
         return candidate.getGender() == User.Gender.FEMALE && next.match(user, candidate);
    }
}
