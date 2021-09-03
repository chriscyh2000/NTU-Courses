package tutorials;

public class StringTutorial {

    public static void main(String[] args) {
        // How to split by spaces
        String line = "A1 B2 C3";
        String[] split = line.split(" "); // [A1, B2, C3]
        for (int i = 0; i < split.length; i++) {
            String token = split[i];              // First Loop | Second Loop | Third Loop |
            System.out.println(token.charAt(0));  //    'A'     |     'B'     |    'C'     |
            System.out.println(token.charAt(1));  //    '1'     |     '2'     |    '3'     |
        }

        // substring
        String c = "0123456789";
        System.out.println(c.substring(0, 7)); // "0123456"
        System.out.println(c.substring(3, 8)); // "34567"

        System.out.println("     <(-W-)>    ".stripLeading());  // "<(-W-)>    "
        System.out.println("     <(-W-)>    ".stripTrailing());  // "     <(-W-)>"
        System.out.println("     <(-W-)>    ".trim());  // "<(-W-)>"

        // Use equals instead of '==' !!
        System.out.println("A" == new String("A"));  // false
        System.out.println("A".equals(new String("A")));  // true



    }
}
