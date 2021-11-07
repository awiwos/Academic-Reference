/*
Name: Andrew Woods
login ID: awwoods
Flush abstract class to represent functions relating to flush conept.
*/

public abstract class Flush{
   private final static int CARDS_PER_HAND = 5;
   private final static int FLUSH_VALUE = 5;

   /*Returns all cards are of the same suit. Otherwise false*/
   public static boolean isFlush(Card[] cards){
      boolean pass = true;
      for(int i=1;i<CARDS_PER_HAND;i++){
         if(cards[i].getSuit()!=cards[0].getSuit()){
            return false;
         }
      }
      return true;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of flush*/

   /*Returns string of the form Xs-high flush where X is the highest rank*/
   public static String getResult(Card[] cards){
      int rank = cards[CARDS_PER_HAND-1].getRank();
      String value = Card.rankToString(rank);
      return String.format("%s-high flush",value);
   }

   /*Tie break given by 1) value of flush, then 2) ranks of cards in
   decending order.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      //first, the value of the high card, then so on with remaining cards
      tieBreak[0] = FLUSH_VALUE;
      for(int i=1;i<CARDS_PER_HAND+1;i++){
         tieBreak[i] = cards[CARDS_PER_HAND - i].getRank();
      }
   }
}
