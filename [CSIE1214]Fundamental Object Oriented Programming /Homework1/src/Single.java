import java.util.ArrayList;

public class Single extends Pattern{
    public Single(){
        super();
    }
    public Single(ArrayList<Card> myCards){
        super(myCards, 1, "single");
    }
    @Override
    public void setCards(ArrayList<Card> myCards){
        setValue(1);
        cards = myCards;
        biggest = myCards.get(myCards.size() - 1);
    }

    @Override
    public boolean isValid(){
        if(cards.size() != 1) return false;
        return true;
    }

    @Override
    public String getPattern(){
        return "single";
    }
}