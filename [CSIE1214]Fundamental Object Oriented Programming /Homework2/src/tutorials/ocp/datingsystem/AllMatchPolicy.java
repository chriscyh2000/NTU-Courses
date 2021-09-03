package tutorials.ocp.datingsystem;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class AllMatchPolicy implements MatchPolicy {
    @Override
    public boolean match(User user, User candidate) {
        return true;
    }
}
