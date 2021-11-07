/*
Name: Andrew Woods
login ID: awwoods
High Card abstract class to represent functions relating to high card conept.
*/

public abstract class HighCard{
   private final static int CARDS_PER_HAND = 5;
   private final static int HIGH_CARD_VALUE = 0;

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of high card*/

   /*Returns string of the form Xs-high where X is highest rank in the hand*/
   public static String getResult(Card[] cards){
      String value = Card.rankToString(cards[CARDS_PER_HAND-1].getRank());
      return String.format("%s-high",value);
   }

   /*Tie break given by 1) value of high card, then ranks of cards in
   decending order.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = HIGH_CARD_VALUE;
      for(int i=1;i<CARDS_PER_HAND+1;i++){
         tieBreak[i] = cards[CARDS_PER_HAND - i].getRank();
      }
   }
}
