public class gameSetting {
    public Deck deck;
    public Player[] player = new Player[4];
    public gameSetting(){
        String name;
        deck = new Deck();
        deck.addCards();
        Inputs.in.nextLine();
        name = Inputs.in.nextLine();
        player[0] = new Player(name, 0);
        name = Inputs.in.nextLine();
        player[1] = new Player(name, 1);
        name = Inputs.in.nextLine();
        player[2] = new Player(name, 2);
        name = Inputs.in.nextLine();
        player[3] = new Player(name, 3);
        deck.distributeCards(player[0], player[1], player[2], player[3]);
        for(int i = 0; i < 4; i++){
            player[i].sortMyHandCards();
        }
    }
}