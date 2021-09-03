package tutorials;

// You can find the details here
// https://docs.oracle.com/javase/7/docs/api/java/util/Formatter.html
public class FormattingTutorial {
    public static void main(String[] args) {
        // Print numbers and strings
        int id = 20;
        String name = "Waterball";
        System.out.printf("My id: %d, My Name: %s\n", id, name); // My id: 20, My Name: Waterball\n

        // Print floats and doubles
        float a = 14.12345f;
        double b = 14.1234567890123456;
        // write %.2f/%.7f to round up to the second/seventh decimal place
        System.out.printf("A: %.2f, B: %.7f\n", a, b);  // A: 14.12, B: 14.1234568\n
        // Scientific notation
        System.out.printf("A: %e, B: %e\n", a, b); // A: 1.412345e+01, B: 1.412346e+01
        System.out.printf("A: %g, B: %g\n", a, b); // A: 1.412345e+01, B: 1.412346e+01


        // Argument index
        // You can specify the argument index as %<index>$<notation>
        System.out.printf("%5$d, %4$d, %3$d, %2$d, %1$d\n", 1, 2, 3, 4, 5); // 5, 4, 3, 2, 1\n

        // Alignment, you can align the content with the field's width
        // Positive width: left alignment, Negative width: right alignment
        // |  1| 20|300|400|50 |6  |
        System.out.printf("|%3d|%3d|%3d|%-3d|%-3d|%-3d|\n", 1, 20, 300, 400, 50, 6);

    }
}
