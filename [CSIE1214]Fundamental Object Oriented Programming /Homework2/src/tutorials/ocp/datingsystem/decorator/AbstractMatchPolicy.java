package tutorials.ocp.datingsystem.decorator;

import tutorials.ocp.datingsystem.MatchPolicy;
import tutorials.ocp.datingsystem.User;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public abstract class AbstractMatchPolicy implements MatchPolicy {
    protected MatchPolicy next = new MatchPolicy() {
        @Override
        public boolean match(User user, User candidate) {
            return true;  // default
        }
    };

    public AbstractMatchPolicy() {
    }

    public AbstractMatchPolicy(MatchPolicy next) {
        this.next = next;
    }

}
