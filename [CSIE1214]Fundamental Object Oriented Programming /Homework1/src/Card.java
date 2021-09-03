import java.util.*;

public class Card implements Comparable<Card> {
    Rank myRank;
    Suit mySuit;

    @Override
    public String toString(){
        return (mySuit.getSymbol() + "[" + myRank.getSymbol() + "]");
    }
    public int compareTo(Card otherCard){
        return (myRank.getOrder() != otherCard.myRank.getOrder()) ?
                myRank.getOrder() - otherCard.myRank.getOrder() :
                mySuit.getOrder() - otherCard.mySuit.getOrder();
    }
}