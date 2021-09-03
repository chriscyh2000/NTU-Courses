public enum Rank {
    THREE(0, "3"),
    FOUR(1, "4"),
    FIVE(2, "5"),
    SIX(3, "6"),
    SEVEN(4, "7"),
    EIGHT(5, "8"),
    NINE(6, "9"),
    TEN(7, "10"),
    JACK(8, "J"),
    QUEEN(9, "Q"),
    KING(10, "K"),
    ACE(11, "A"),
    TWO(12, "2");

    Rank(int order, String symbol){
        this.order = order;
        this.symbol = symbol;
    }
    private int order;
    private String symbol;

    public int getOrder(){
        return order;
    }
    public String getSymbol(){
        return symbol;
    }
    public static Rank getRank(String symbol){
        Rank toReturn = Rank.THREE;
        switch (symbol){
            case "3":
                toReturn = Rank.THREE;
                break;
            case "4":
                toReturn = Rank.FOUR;
                break;
            case "5":
                toReturn = Rank.FIVE;
                break;
            case "6":
                toReturn = Rank.SIX;
                break;
            case "7":
                toReturn = Rank.SEVEN;
                break;
            case "8":
                toReturn = Rank.EIGHT;
                break;
            case "9":
                toReturn = Rank.NINE;
                break;
            case "1":
                toReturn = Rank.TEN;
                break;
            case "J":
                toReturn = Rank.JACK;
                break;
            case "Q":
                toReturn = Rank.QUEEN;
                break;
            case "K":
                toReturn = Rank.KING;
                break;
            case "A":
                toReturn = Rank.ACE;
                break;
            case "2":
                toReturn = Rank.TWO;
                break;
            default:
                System.out.println("Invalid Rank.");
        }
        return toReturn;
    }
}