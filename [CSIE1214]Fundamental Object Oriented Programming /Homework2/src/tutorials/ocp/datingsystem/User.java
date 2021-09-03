package tutorials.ocp.datingsystem;

import java.util.HashSet;
import java.util.Set;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class User {
    // only the matched users will be your friends
    private final Set<User> friends = new HashSet<>();
    private MatchPolicy matchPolicy = new AllMatchPolicy(); // default policy
    private final Gender gender;
    private final String name;
    private final String description;
    private final int salary;

    public User(Gender gender, String name, String description, int salary) {
        this.gender = gender;
        this.name = name;
        this.description = description;
        this.salary = salary;
    }

    public enum Gender {
        FEMALE, MALE
    }

    public void match(User ...candidates) {
        for (User candidate : candidates) {
            match(candidate);
        }
    }

    public void match(User candidate) {
        if (matchPolicy.match(this, candidate)) {
            friends.add(candidate);
            candidate.getFriends().add(this);
        }
    }

    public Set<User> getFriends() {
        return friends;
    }

    public Gender getGender() {
        return gender;
    }

    public String getName() {
        return name;
    }

    public String getDescription() {
        return description;
    }

    public int getSalary() {
        return salary;
    }

    public void setMatchPolicy(MatchPolicy matchPolicy) {
        this.matchPolicy = matchPolicy;
    }
}

