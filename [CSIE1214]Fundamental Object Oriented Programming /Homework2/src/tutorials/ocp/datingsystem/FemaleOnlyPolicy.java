package tutorials.ocp.datingsystem;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class FemaleOnlyPolicy implements MatchPolicy {
    @Override
    public boolean match(User user, User candidate) {
        return candidate.getGender() == User.Gender.FEMALE;
    }
}
