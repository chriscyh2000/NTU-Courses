package tutorials.ocp.datingsystem.decorator;

import tutorials.ocp.datingsystem.MatchPolicy;
import tutorials.ocp.datingsystem.User;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class LongText extends AbstractMatchPolicy {
    public LongText() {
    }

    public LongText(MatchPolicy next) {
        super(next);
    }

    @Override
    public boolean match(User user, User candidate) {
        return candidate.getDescription().length() >= 10 && next.match(user, candidate);
    }
}
