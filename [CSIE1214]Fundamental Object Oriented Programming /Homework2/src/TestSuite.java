import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;

public class TestSuite {
    public static final String TESTCASES_DIR = "testcases";
    private static final PrintStream stdOut = System.out;
    private static String expectedOutput;
    private static ByteArrayOutputStream programOutput;

    /**
     * Compile and run this TestSuite:
     * 1. cd src/
     * 2. javac TestSuite.java -d ../out/
     * 3. cd ..
     * 4. java -cp out/ TestSuite <testcase name>
     *
     * Or you can directly run `sh test.sh`.
     * The script helps you do all the above.
     */
    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.out.println("You must provide the test name as the first argument.");
        }
        String testName = args[0];
        System.out.printf("\n\n########### Test: %s ######################\n\n", testName);
        setupIO(testName);
        try {
            Main.main(new String[0]);
        } catch (Exception err) {
            err.printStackTrace();
        } finally {
            judge();
        }
    }

    private static void setupIO(String testName) throws IOException {
        expectedOutput = Files.readString(Paths.get(TESTCASES_DIR).resolve(testName + ".out"));
        String input = Files.readString(Paths.get(TESTCASES_DIR).resolve(testName + ".in"));
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        programOutput = new ByteArrayOutputStream();
        System.setOut(new PrintStream(programOutput, true));
    }

    private static void judge() {
        String actualOutput = programOutput.toString();
        System.setOut(stdOut);  // restore back to the standard output
        String[] actualOutputLines = actualOutput.stripTrailing().split("\n");
        String[] expectedOutputLines = expectedOutput.stripTrailing().split("\n");
        boolean allCorrect = compareLineByLine(actualOutputLines, expectedOutputLines);
        if (allCorrect) {
            System.out.println("All Correct!");
        } else {
            System.exit(-1);
        }
    }

    private static boolean compareLineByLine(String[] actualOutputLines, String[] expectedOutputLines) {
        int n = Math.min(expectedOutputLines.length, actualOutputLines.length);
        for (int i = 0; i < n; i++) {
            String actual = actualOutputLines[i].stripTrailing();
            String expected = expectedOutputLines[i].stripTrailing();
            if (!actual.equals(expected)) {
                printTestReport(actualOutputLines, expectedOutputLines, i + 1);
                return false;
            }
        }
        if (actualOutputLines.length > expectedOutputLines.length) {
            printTestReport(actualOutputLines, expectedOutputLines, expectedOutputLines.length + 1);
            return false;
        }
        if (expectedOutputLines.length > actualOutputLines.length) {
            printTestReport(actualOutputLines, expectedOutputLines, actualOutputLines.length + 1);
            return false;
        }
        return true;
    }

    private static void printTestReport(String[] actualOutputLines, String[] expectedOutputLines, int errorLine) {
        System.out.println("======== Expected Output ========");
        printWithLineNumbers(errorLine, expectedOutputLines);
        System.out.println("\n======== Actual Output ========");
        printWithLineNumbers(errorLine, actualOutputLines);
        System.err.printf("\nWrong answer, the difference starts at line %d.\n", errorLine);
        if (errorLine <= Math.min(expectedOutputLines.length, actualOutputLines.length)) {
            System.err.printf("Expected: %s\n  Actual: %s\n",
                    expectedOutputLines[errorLine - 1].stripTrailing(),
                    actualOutputLines[errorLine - 1].stripTrailing());
        }
    }

    private static void printWithLineNumbers(int errorLine, String[] lines) {
        for (int i = 1; i <= lines.length; i++) {
            String line = errorLine == i ? "*" + i : String.valueOf(i);
            System.out.printf("%4s| %s\n", line, lines[i - 1]);
        }
    }
}

