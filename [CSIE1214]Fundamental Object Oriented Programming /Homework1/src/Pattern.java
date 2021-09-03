import java.util.ArrayList;
import java.util.Collections;

public class Pattern {
    private int value;
    private String PatternName;
    ArrayList<Card> cards;
    Card biggest;
    public Pattern(){
        cards = new ArrayList<Card>();
    }
    public Pattern(ArrayList<Card> myCards, int i, String pName){
        PatternName = pName;
        value = i;
        cards = myCards;
        biggest = cards.get(cards.size() - 1);
        Collections.sort(cards);
    }
    public void setCards(ArrayList<Card> myCards){};
    public boolean isValid(){return false;};
    public boolean isGreater(Pattern other){
        if(value != other.value){
            return false;
        }
        if(biggest.myRank.getOrder() > other.biggest.myRank.getOrder())
            return true;
        if(biggest.myRank.getOrder() == other.biggest.myRank.getOrder()){
            if(biggest.mySuit.getOrder() > other.biggest.mySuit.getOrder())
                return true;
            else return false;
        }
        return false;
    }
    public void printAllCards(){
        for (Card c : cards)
            System.out.print(" " + c);
        System.out.print(".\n");
    }
    public String getPattern(){return PatternName;};
    public int getValue(){
        return value;
    }
    public void setValue(int i){
        value = i;
    }
}
