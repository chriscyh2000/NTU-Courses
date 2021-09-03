import java.util.ArrayList;
import java.util.Collections;
public class Player {
    int index;
    boolean isGameStarter;
    boolean isNewRoundStarter;
    private String myName;
    private HandCards myHandCards;

    public Player(String name, int idx){
        isGameStarter = false;
        isNewRoundStarter = false;
        index = idx;
        myName = name;
        myHandCards = new HandCards();
    }
    public String getMyName(){
        return myName;
    }
    public HandCards getMyHandCards(){
        return  myHandCards;
    }
    public void receiveCards(Card toAdd){
        myHandCards.addCards(toAdd);
    }
    public void sortMyHandCards(){
        Collections.sort(myHandCards.getHandCards());
    }
    public ArrayList<Card> takeActions(int[] command){
        ArrayList<Card> outCards = new ArrayList<Card>();
        for(int i : command){
            outCards.add(myHandCards.getCard(i));
        }
        for(Card c : outCards){
            myHandCards.removeCards(c);
        }
        Collections.sort(outCards);
        return outCards;
    }
}
