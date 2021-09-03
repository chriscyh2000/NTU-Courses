package tutorials;

import java.io.ByteArrayInputStream;
import java.util.Scanner;

public class ScannerTutorial {
    private final static String input =
            "1 2 3\n" +
            "aa bb cc\n" +
            "line1\n" +
            "line2";

    // Typically you should use `new Scanner(System.in)` to consume from the standard input,
    // However, in this tutorial, we directly feed the 'input' String into the Scanner.
    private final static Scanner scanner = new Scanner(new ByteArrayInputStream(input.getBytes()));

    public static void main(String[] args) {
        System.out.println(scanner.nextInt()); // 1
        System.out.println(scanner.nextInt()); // 2
        System.out.println(scanner.nextInt()); // 3
        System.out.println(scanner.next()); // "aa"
        System.out.println(scanner.next()); // "bb"
        System.out.println(scanner.next()); // "cc"

        // Notice! You must consume the line-break '\n' that follows 'cc'
        // since `nextInt() and next() don't consume the trailing line-break.
        scanner.nextLine();  // ""

        System.out.println(scanner.nextLine()); // "line1"
        System.out.println(scanner.nextLine()); // "line2"
    }
}
