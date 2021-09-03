import java.util.ArrayList;
public class Straight extends Pattern{
    public Straight(){
        super();
    }
    public Straight(ArrayList<Card> myCards){
        super(myCards, 3, "straight");
    }


    public void setCards(ArrayList<Card> myCards){
        setValue(3);
        cards = myCards;
        biggest = myCards.get(myCards.size() - 1);
    }

    @Override
    public boolean isValid(){
        if(cards.size() != 5) return false;

        for(int i = 0; i < 4; i++){
            int cur = cards.get(i).myRank.getOrder();
            int next = cards.get(i + 1).myRank.getOrder();
            if(next - cur != 9 && next - cur != 1) return false;
        }
        return true;
    }

    @Override
    public String getPattern(){
        return "straight";
    }
}
