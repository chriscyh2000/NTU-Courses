package tutorials;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class CollectionsApiTutorial {

    public static void main(String[] args) {
        List<Integer> numbers = Arrays.asList(1, 3, 2, 4, 2, 5, 2, 6);

        // The default soring algorithm is `TimSort`
        Collections.sort(numbers); // result in [1, 2, 2, 2, 3, 4, 5, 6]

        System.out.println(Collections.binarySearch(numbers, 4));  // 5
        System.out.println(Collections.frequency(numbers, 2)); // 3
        System.out.println(Collections.min(numbers)); // 1
        System.out.println(Collections.max(numbers)); // 6

        Collections.rotate(numbers, 1); // result in [6, 1, 2, 2, 2, 3, 4, 5]
        // rotate back
        Collections.rotate(numbers, 7); // result in [1, 2, 2, 2, 3, 4, 5, 6]
        Collections.reverse(numbers); // result in [6, 5, 4, 3, 2, 2, 2, 1]
        Collections.swap(numbers, 0, 1); // result in [5, 6, 4, 3, 2, 2, 2, 1]

        System.out.println(Collections.indexOfSubList(numbers, Arrays.asList(2, 2))); // 4
        System.out.println(Collections.lastIndexOfSubList(numbers, Arrays.asList(2, 2)));  // 5

        Collections.shuffle(numbers); // you can shuffle the list in only one line of code
        Collections.fill(numbers, 1); // result in [1, 1, 1, 1, 1, 1, 1, 1]

    }
}
