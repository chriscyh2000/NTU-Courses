package tutorials.ocp.datingsystem;

/**
 * What kinds of person would you like to date with?
 *
 * @author - johnny850807@gmail.com (Waterball)
 */
public interface MatchPolicy {

    boolean match(User user, User candidate);
}
