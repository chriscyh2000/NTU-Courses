import java.util.ArrayList;
public class FullHouse extends Pattern{
    public FullHouse (){
        super();
    }
    public FullHouse(ArrayList<Card> myCards){
        super(myCards, 4, "full house");
    }
    @Override
    public boolean isValid(){
        if(cards.size() != 5) return false;

        Card mid = cards.get(2);
        if(cards.get(0).myRank.getOrder() == mid.myRank.getOrder()){
            if(cards.get(0).myRank.getOrder() != cards.get(1).myRank.getOrder())
                return false;
            if(cards.get(3).myRank.getOrder() != cards.get(4).myRank.getOrder())
                return false;
            return true;
        }
        else if(cards.get(3).myRank.getOrder() == mid.myRank.getOrder()){
            if(cards.get(0).myRank.getOrder() != cards.get(1).myRank.getOrder())
                return false;
            if(cards.get(3).myRank.getOrder() != cards.get(4).myRank.getOrder())
                return false;
            return true;
        }
        else return false;
    }

    @Override
    public void setCards(ArrayList<Card> myCards){
        setValue(4);
        cards = myCards;
        biggest = myCards.get(myCards.size() - 1);
    }

    @Override
    public boolean isGreater(Pattern other) {
        if(super.getValue() != other.getValue()) return false;
        if(cards.get(2).myRank.getOrder() < other.cards.get(2).myRank.getOrder())
            return false;
        return true;
    }

    @Override
    public String getPattern(){
        return "full house";
    }
}