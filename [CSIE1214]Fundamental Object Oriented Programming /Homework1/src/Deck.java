import java.util.Stack;

public class Deck {
    public Stack<Card> shuffleDeck;
    private int startIdx;
    private Player topPlayer;
    private Pattern topPlay;

    public Deck(){
        shuffleDeck = new Stack<Card>();
    }

    public void setTopPlayer(Player p){
        topPlayer = p;
    }
    public Player getTopPlayer(){
        return topPlayer;
    }

    public void setTopPlay(Pattern top){
        topPlay = top;
    }
    public Pattern getTopPlay(){
        return topPlay;
    }
    public int getStartIdx(){
        return startIdx;
    }

    public void addCards(){
        String toAdd;
        for(int i = 0; i < 52; i++){
            toAdd = Inputs.in.next();
            Card newCard = new Card();
            newCard.mySuit = Suit.getSuit(toAdd.substring(0,1));
            newCard.myRank = Rank.getRank(toAdd.substring(2,3));
            shuffleDeck.push(newCard);
        }
    }
    public void distributeCards(Player p0, Player p1, Player p2, Player p3){
        for(int i = 0; i < 52; i++){
            if(i % 4 == 0){
                if (shuffleDeck.peek().myRank.getOrder() == 0 &&
                        shuffleDeck.peek().mySuit.getOrder() == 0){
                    topPlayer = p0;
                    p0.isGameStarter = true;
                    p0.isNewRoundStarter = true;
                    startIdx = 0;
                }
                p0.receiveCards(shuffleDeck.pop());
            }
            else if(i % 4 == 1){
                if (shuffleDeck.peek().myRank.getOrder() == 0 &&
                        shuffleDeck.peek().mySuit.getOrder() == 0){
                    topPlayer = p1;
                    p1.isGameStarter = true;
                    p1.isNewRoundStarter = true;
                    startIdx = 1;
                }
                p1.receiveCards(shuffleDeck.pop());
            }
            else if(i % 4 == 2){
                if (shuffleDeck.peek().myRank.getOrder() == 0 &&
                        shuffleDeck.peek().mySuit.getOrder() == 0){
                    topPlayer = p2;
                    p2.isGameStarter = true;
                    p2.isNewRoundStarter = true;
                    startIdx = 2;
                }
                p2.receiveCards(shuffleDeck.pop());
            }
            else{
                if (shuffleDeck.peek().myRank.getOrder() == 0 &&
                        shuffleDeck.peek().mySuit.getOrder() == 0){
                    topPlayer = p3;
                    p3.isGameStarter = true;
                    p3.isNewRoundStarter = true;
                    startIdx = 3;
                }
                p3.receiveCards(shuffleDeck.pop());
            }
        }
    }
}
