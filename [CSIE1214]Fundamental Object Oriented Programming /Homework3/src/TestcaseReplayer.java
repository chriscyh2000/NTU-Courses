import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

/**
 * @author - johnny850807@gmail.com (Waterball)
 */
public class TestcaseReplayer {
    public static final String TEXT_RESET = "\u001B[0m";
    public static final String TEXT_RED = "\u001B[31m";
    public static final String TEXT_YELLOW = "\u001B[33m";
    public static final String TEXT_CYAN = "\u001B[36m";
    public static final String TESTCASES_DIR = "testcases";

    public List<String> replay(List<String> inputs, List<String> outputs) {
        List<String> replay = new ArrayList<>();
        var in = inputs.iterator();
        var out = outputs.iterator();
        String output = out.next();
        String input = in.next();
        while (!input.equals("exit")) {
            replay.add(String.format("%s> %s",
                    TEXT_CYAN + output.replace("Current path: ", "").trim(),
                    TEXT_YELLOW + input));
            output = out.next();
            while (!output.startsWith("Current path: ")) {
                String color = output.startsWith("Illegal") ? TEXT_RED : TEXT_RESET;
                replay.add(color + output);
                output = out.next();
            }
            input = in.next();
        }
        replay.add(String.format("%s> %s",
                TEXT_CYAN + output.replace("Current path: ", "").trim(),
                TEXT_YELLOW + input) + TEXT_RESET);
        return replay;
    }

    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.out.println("You must provide the test name as the first argument.");
        }
        String testName = args[0];
        var in = Files.readAllLines(Paths.get(TESTCASES_DIR).resolve(testName + ".in"));
        var out = Files.readAllLines(Paths.get(TESTCASES_DIR).resolve(testName + ".out"));

        TestcaseReplayer replayer = new TestcaseReplayer();
        var replay = replayer.replay(in, out);
        replay.forEach(System.out::println);
    }
}
