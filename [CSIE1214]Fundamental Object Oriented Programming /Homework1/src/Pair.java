import java.util.ArrayList;

public class Pair extends Pattern{
    public Pair(){
        super();
    }
    public Pair(ArrayList<Card> myCards){
        super(myCards, 2, "pair");
    }

    @Override
    public void setCards(ArrayList<Card> myCards){
        setValue(2);
        cards = myCards;
        biggest = myCards.get(myCards.size() - 1);
    }

    @Override
    public boolean isValid() {
        if(cards.size() != 2) return false;
        if(cards.get(0).myRank.getOrder() != cards.get(1).myRank.getOrder())
            return false;
        return true;
    }

    @Override
    public String getPattern(){
        return "pair";
    }
}