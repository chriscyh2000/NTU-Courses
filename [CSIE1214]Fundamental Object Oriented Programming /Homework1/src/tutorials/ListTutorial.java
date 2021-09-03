package tutorials;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ListTutorial {
    public static void main(String[] args) {
        // ArrayList is a type of List
        List<String> numbers = new ArrayList<>();  // or `new LinkedList<>()`

        System.out.println(numbers.isEmpty());  // true

        // add numbers
        numbers.add("1");
        numbers.add("2");
        numbers.add("3");
        numbers.add("4");
        numbers.add("5");

        System.out.println(numbers.isEmpty());  // false

        // traverse using for-loop
        for (int i = 0; i < numbers.size(); i++) {
            System.out.println(numbers.get(i));
        }
        
        // traverse using for-each iterator (This is much prettier)
        for (String number : numbers) {
            System.out.println(number);
        }

        // A convenient method of constructing an ArrayList
        List<String> anotherNumbers  = Arrays.asList("6", "7", "8", "9", "10");

        // You can add another ArrayList directly via `addAll`
        // then numbers will contain ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10"]
        numbers.addAll(anotherNumbers);

        numbers.remove(3);  // result in ["1", "2", "3", "5", "6", "7", "8", "9", "10"]
        numbers.remove("7"); // result in ["1", "2", "3", "5", "6", "8", "9", "10"]
        numbers.set(5, "<(-W-)>"); // result in ["1", "2", "3", "5", "6", "<(-W-)>", "9", "10"]
        numbers = numbers.subList(1, 5);  // result in ["2", "3", "5", "6"]

        System.out.println(numbers.contains("1"));  // false
        System.out.println(numbers.contains("2"));  // true

        numbers.add("2"); // result in ["2", "3", "5", "6", "2"]
        numbers.retainAll(Arrays.asList("3", "2")); // result in ["2", "3", "2"]

        System.out.println(numbers.indexOf("2")); // 0
        System.out.println(numbers.indexOf("<(-W-)>"));  // -1 (Not found)
        System.out.println(numbers.lastIndexOf("2"));  // 2
        System.out.println(numbers.containsAll(Arrays.asList("1", "2"))); // false
        System.out.println(numbers.containsAll(Arrays.asList("2", "3"))); // true


        // You can convert the list into array like this
        String[] numberArrays = numbers.toArray(new String[0]); // ["2", "3", "2"]

        numbers.clear(); // result in []
    }
}
