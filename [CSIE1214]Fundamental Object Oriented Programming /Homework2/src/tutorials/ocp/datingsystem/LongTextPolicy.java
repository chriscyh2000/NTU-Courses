package tutorials.ocp.datingsystem;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class LongTextPolicy implements MatchPolicy {
    @Override
    public boolean match(User user, User candidate) {
        return candidate.getDescription().length() >= 10;
    }
}
