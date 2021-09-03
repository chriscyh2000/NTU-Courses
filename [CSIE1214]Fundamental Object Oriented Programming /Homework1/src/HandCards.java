import java.util.ArrayList;
import java.util.Collections;

public class HandCards {
    private ArrayList<Card> handCards;
    public HandCards(){
        this.handCards = new ArrayList<Card>();
    }
    public ArrayList<Card> getHandCards(){
        return handCards;
    }
    public Card getCard(int i){
        return handCards.get(i);
    }
    public void sortHandCards(){
        Collections.sort(handCards);
    }
    public void addCards(Card card){
        handCards.add(card);
    }
    public void removeCards(Card card){
        handCards.remove(card);
    }
    public void showHandCards(){
        StringBuilder numbers = new StringBuilder();
        StringBuilder cards = new StringBuilder();
        for (int i = 0; i < handCards.size(); i++) {
            String card = handCards.get(i).toString();
            numbers.append(String.format("%"+(-card.length())+"s", i)).append(" ");
            cards.append(card).append(" ");
        }
        System.out.println(numbers.toString().stripTrailing());
        System.out.println(cards.toString().stripTrailing());
    }
}
