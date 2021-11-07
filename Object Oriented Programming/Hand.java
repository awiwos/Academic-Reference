/*
Name: Andrew Woods
login ID: awwoods
Hand class represents player's hands and functions relating thereto.
*/

public class Hand{
   private final static int CARDS_PER_HAND = 5;
   private final static int TIEBREAKS = 6;
   private final int playerID;
   private String result;
   private Card[] cards = new Card[5];
   private int[] tieBreak = new int[6];

   /*Constructor to instantiate hand with cards from low to high in args*/
   Hand(int playerNumber, String[] args, int low, int high){
      //Initialise tiebreak with zeros.
      for(int i=0;i<TIEBREAKS;i++){
         tieBreak[i] = 0;
      }
      //set player ID number
      this.playerID = playerNumber;

      //instantiate the five cards associated with the hand
      for(int i=low;i<high;i++){
         cards[i-low] = new Card(args[i].toUpperCase());
      }
      //sort the cards based on rank
      Card.sortCards(cards);
      //evaluate the hand i.e. find result string and tiebreak values.
      evaluateHand();
   }

   /*Evaluate the hand i.e. find the result string, and calculate tiebreak
   values*/
   private void evaluateHand(){
      //Assess what the result of the hand is by checking if the cards fit
      //the result in decending order of result. If result found, then,
      //determine result string as well as tiebeak values.
      if(StraightFlush.isStraightFlush(cards)){
         this.result = StraightFlush.getResult(cards);
         StraightFlush.getTieBreak(tieBreak, cards);

      } else if (FourOfAKind.isFourOfAKind(cards)){
         this.result = FourOfAKind.getResult(cards);
         FourOfAKind.getTieBreak(tieBreak, cards);

      } else if (FullHouse.isFullHouse(cards)){
         this.result = FullHouse.getResult(cards);
         FullHouse.getTieBreak(tieBreak, cards);

      } else if (Flush.isFlush(cards)){
         this.result = Flush.getResult(cards);
         Flush.getTieBreak(tieBreak, cards);

      } else if (Straight.isStraight(cards)){
         this.result = Straight.getResult(cards);
         Straight.getTieBreak(tieBreak, cards);

      } else if (ThreeOfAKind.isThreeOfAKind(cards)){
         this.result = ThreeOfAKind.getResult(cards);
         ThreeOfAKind.getTieBreak(tieBreak, cards);

      } else if (TwoPair.isTwoPair(cards)){
         this.result = TwoPair.getResult(cards);
         TwoPair.getTieBreak(tieBreak, cards);

      } else if (Pair.isPair(cards)){
         this.result = Pair.getResult(cards);
         Pair.getTieBreak(tieBreak, cards);

      } else {
         this.result = HighCard.getResult(cards);
         HighCard.getTieBreak(tieBreak, cards);
      }
   }

   /*Getter - returns result string for hand*/
   public String getResult(){
      return this.result;
   }

   /*Getter - returns player ID for hand*/
   public int getID(){
      return this.playerID;
   }

   /*Getter - returns tiebreak for hand*/
   public int[] getTieBreak(){
      return this.tieBreak;
   }

}
