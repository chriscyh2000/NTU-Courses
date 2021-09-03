public enum Suit {
    CLUB(0, "C"),
    DIAMOND(1, "D"),
    HEART(2, "H"),
    SPADE(3, "S");

    Suit (int order, String symbol){
        this.order = order;
        this.symbol = symbol;
    }
    private int order;
    private String symbol;

    public int getOrder(){
        return order;
    }
    public String getSymbol() {
        return symbol;
    }
    public static Suit getSuit(String symbol){
        Suit toReturn = Suit.CLUB;
        switch (symbol){
            case "C":
                toReturn = Suit.CLUB;
                break;
            case "D":
                toReturn = Suit.DIAMOND;
                break;
            case "H":
                toReturn = Suit.HEART;
                break;
            case "S":
                toReturn = Suit.SPADE;
                break;
            default:
                System.out.println("Invalid Suit.");
        }
        return toReturn;
    }
}
