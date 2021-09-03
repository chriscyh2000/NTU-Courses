package tutorials;


public class HowDoIPrintMyHandCards {
    public static void main(String[] args) {
        // You can align your hand-cards like this,
        // where each card index is strictly left-aligned with the beginning of each card,
        // and every card is separate by a space.
        // 0    1     2    3     4    5    6      7    8    9     10
        // A[1] B[20] C[3] D[40] E[5] F[7] G[000] H[7] I[9] J[10] K[88]

        // Given your arbitrary hand-cards.
        String[] handCards = {"A[1]", "B[20]", "C[3]", "D[40]" ,"E[5]", "F[7]", "G[000]", "H[7]", "I[9]", "J[10]", "K[88]"};

        StringBuilder numbers = new StringBuilder();
        StringBuilder cards = new StringBuilder();
        for (int i = 0; i < handCards.length; i++) {
            String card = handCards[i];
            numbers.append(String.format("%"+(-card.length())+"s", i)).append(" ");
            cards.append(card).append(" ");
        }

        // the following will print
        System.out.println(numbers.toString().stripTrailing());  // strip the trailing whitespaces
        System.out.println(cards.toString().stripTrailing()); // strip the trailing whitespaces
    }
}
