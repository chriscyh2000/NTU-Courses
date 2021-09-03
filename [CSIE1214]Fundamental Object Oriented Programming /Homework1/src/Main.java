import java.util.ArrayList;
/**
 * The big-2 game will be started from the Main class.
 */
public class Main {
    public static void executeNewGame(Player[] player, Deck deck, int startIdx) {
        ArrayList<Pattern> testPattern = new ArrayList<Pattern>();
        Single testSingle = new Single();
        Pair testPair = new Pair();
        Straight testStraight = new Straight();
        FullHouse testFullHouse = new FullHouse();
        //if you want to add new patterns, just add a line "<New Pattern> test<NewPattern> = new <New Pattern>"
        testPattern.add(testSingle);
        testPattern.add(testPair);
        testPattern.add(testStraight);
        testPattern.add(testFullHouse);
        //if you want to add new patterns, just add a line "testPattern.add(test<New Pattern>)"
        String winner;
        int i = startIdx;
        int contiPass = 0;
        ArrayList<Card> outCard;
        Pattern outPlay = new Single();
        while(true){
            if(player[i%4].isNewRoundStarter){
                System.out.println("New round begins.");
                System.out.println("Next turn: " + player[i%4].getMyName());
                boolean outCardsAreValid = false;

                while(!outCardsAreValid){
                    player[i%4].getMyHandCards().showHandCards();
                    int[] command;
                    String[] strCommand;
                    strCommand = Inputs.in.nextLine().split(" ");
                    command = new int[strCommand.length];
                    for (int j = 0; j < strCommand.length; j++)
                        command[j] = Integer.parseInt(strCommand[j]);

                    if(command[0] == -1){
                        System.out.println("You can't pass in the new round.");
                        continue;
                    }

                    outCard = player[i%4].takeActions(command);

                    if(player[i%4].isGameStarter){
                        if(outCard.get(0).myRank.getOrder() != 0 || outCard.get(0).mySuit.getOrder() != 0){
                            System.out.println("Invalid play, please try again.");
                            while(!outCard.isEmpty()){
                                player[i%4].receiveCards(outCard.get(0));
                                outCard.remove(0);
                            }
                            player[i%4].sortMyHandCards();
                            continue;
                        }
                    }

                    for(int j = 0; j < testPattern.size(); j++){
                        Pattern test = testPattern.get(j);
                        test.setCards(outCard);
                        if(test.isValid()){
                            outCardsAreValid = true;
                            outPlay = new Pattern(outCard, test.getValue(), test.getPattern());
                            deck.setTopPlay(outPlay);
                            System.out.print("Player " + player[i%4].getMyName() +
                                    " plays a " + outPlay.getPattern());
                            outPlay.printAllCards();
                            break;
                        }
                    }
                    if(!outCardsAreValid) {
                        System.out.println("Invalid play, please try again.");
                        while(!outCard.isEmpty()){
                            player[i%4].receiveCards(outCard.get(0));
                            outCard.remove(0);
                        }
                        player[i%4].sortMyHandCards();
                    }
                }
                deck.setTopPlayer(player[i%4]);
                deck.getTopPlayer().isNewRoundStarter = false;
                if(deck.getTopPlayer().isGameStarter) deck.getTopPlayer().isGameStarter = false;
            }
            else{
                System.out.println("Next turn: " + player[i%4].getMyName());
                boolean outCardsAreValid = false;

                while(!outCardsAreValid){
                    player[i%4].getMyHandCards().showHandCards();
                    int[] command;
                    String[] strCommand;
                    strCommand = Inputs.in.nextLine().split(" ");
                    command = new int[strCommand.length];
                    for (int j = 0; j < strCommand.length; j++)
                        command[j] = Integer.parseInt(strCommand[j]);

                    if(command[0] == -1){
                        contiPass++;
                        System.out.println("Player " + player[i%4].getMyName() + " passes.");
                        break;
                    }

                    outCard = player[i%4].takeActions(command);

                    for(int j = 0; j < testPattern.size(); j++){
                        Pattern test = testPattern.get(j);
                        test.setCards(outCard);
                        if(test.isValid()){
                            if(test.isGreater(deck.getTopPlay())){
                                outCardsAreValid = true;
                                outPlay = new Pattern(outCard, test.getValue(), test.getPattern());
                                deck.setTopPlay(outPlay);
                                deck.setTopPlayer(player[i%4]);
                                System.out.print("Player " + player[i%4].getMyName() +
                                        " plays a " + outPlay.getPattern());
                                outPlay.printAllCards();
                                contiPass = 0;
                            }
                            break;
                        }
                    }
                    if(!outCardsAreValid) {
                        System.out.println("Invalid play, please try again.");
                        while(!outCard.isEmpty()){
                            player[i%4].receiveCards(outCard.get(0));
                            outCard.remove(0);
                        }
                        player[i%4].sortMyHandCards();
                    }
                }
            }

            if(deck.getTopPlayer().getMyHandCards().getHandCards().size() == 0){
                winner = deck.getTopPlayer().getMyName();
                break;
            }
            if(contiPass == 3){
                deck.getTopPlayer().isNewRoundStarter = true;
                contiPass = 0;
            }
            i++;
        }
        System.out.println("Game over, the winner is " + winner + ".");
    }
    public static void main(String[] args) {
        gameSetting setUp = new gameSetting();
        Main.executeNewGame(setUp.player, setUp.deck, setUp.deck.getStartIdx());
    }
}