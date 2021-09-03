# Design-Report.md
## NTU CSIE FOOP Spring 2021  <br><font size = 3px>B08902149  資工二 徐晨祐</font>
> ### Software Design :
- **Inputs : Contorl all input stream.**
- **Rank : Record the order of every kind of rank.**
    - To tell every card their own rank by `getRank(symbol)` when the card tells this enumerator its symbol. (ex. If the symbol is `"2"` then return `TWO`)
    - Every Card can return the order of its rank according its Rank object. (ex. `C[3]` can use `THREE` to return 0, which is the order of `"3"`, by calling `getOrder()`.)
- **Suit : Record the order of every kind of suit.**
    - To tell every card their own suit by `getSuit(symbol)` when the card tells this enumerator its symbol. (ex. If the symbol is `"S"` then return `SPADE`)
    - Every Card can return the order of its rank according its Rank object. (ex. `C[3]` can use `CLUB` to return 0, which is the order of `"C"`, by calling `getOrder()`.)
- **Card : Creat poker cards as objects.**
    - For every `Card` object , it stores two object. One is `myRank`, the other is `mySuit`. We can use them to get the rank and suit of a card.
- **HandCards : Store an array list: `ArrayList<Card> handCards` to record what cards are in hand.**
    - Use `getcard(i)`to get the i*th* card in the player's handcards.
    - Use `addCards(card)` to add a card into the list of the handcards.
    - Use `removeCards(card)`to remove one of the handcards.
    - Use `sortHandCards()`to sort the handcards.
    - Use `showHandCards()`to print all the handcards with their index above.
- **Pattern : Set up all the common information and methods among all the patterns.** 
    - Use `value`to store the type of the pattern.(ex. if the pattern is single, set `value` as `1`)
    - Use `setCards(myCards)`to update the list of the cards stored in the pattern.
    - Use `isValid()`to check whether the cards stored inside is a valid pattern when this `Pattern`object is set to a certain pattern.
    - Use `isGreater(other)`to check whether these cards is greater than another play when they're the same pattern.
    - Use `printAllCards()`to print all the cards in this pattern when the players release cards validly.
    - All the patterns below extend the class `Pattern` and override some method when the method need to have some special function:
        >- `Single`
        >- `Pair`
        >- `Straight`
        >- `FullHouse`       
- **Player : Construct all the action a player may take in the games.**
    - Use `isGameStarter`to record if the player is the game starter.
    - Use `isNewRoundStarter`to record if the player is the starter of a new round.
    - Use `receiveCards(toAdd)`to add cards in when the deck is distributing cards.
    - Use `takeActions(command)`to take cards out accoreding to the index in `command`, and return all the chosen cards.
- **Deck : Record the current situation on the game table.**
    - Use `topPlayer`and `topPlay`to record who is the top player and record the top play.(When the player who release cards validly just now, he/she will put cards on the deck and it will record who the player is.)
    - Use `addCards()`to read shuffle deck and use `distributeCards(p1, p2, p3, p4)`to distribute cards player by player (During the distributing procedure, the deck will check if each card is `C[3]`and set the player who receive `C[3]`as the game starter).
- **gameSetting : Handle all the procedure to start the new game and initilize the players and the deck.**
- **Main : Control the game flow and follow all the game rules.**
    - To initialize an `ArrayList <Pattern>`called `testPattern`to put a list of `Pattern`objects to check if all the released cards from the players are valid and greater than the top play.
> ### Bonus Part : How to add new patterns while following Open-Closed Principle (OCP) ?
 1. Add a new class of the new pattern and extends the `Pattern` class. Set the `value` of this new pattern differnet from the values of the existing patterns. Last, we have to set the methods `isValid()` and `isGreater(other)` correctly according to what new pattern we want to add in.
 2. To initialize a new `Pattern` object and add it into the array list `testPattern` in the `Main` class. And now we add the new pattern successfully.