/*
Name: Andrew Woods
login ID: awwoods
Staight flush abstract class to represent functions relating to straight
flush conept.
*/

public abstract class StraightFlush{
   private final static int CARDS_PER_HAND = 5;
   private final static int STRAIGHT_FLUSH_VALUE = 8;

   /*Returns true if all cards of a signle suit, and ranks are sequential
   Otherwise false*/
   public static boolean isStraightFlush(Card[] cards){
      if(Straight.isStraight(cards)&&Flush.isFlush(cards)){
         return true;
      }
      return false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of straight flush*/

   /*Returns string of the form Xs-high straight flush where X is the
   highest rank in the hand*/
   public static String getResult(Card[] cards){
      int rank = cards[CARDS_PER_HAND-1].getRank();
      String value = Card.rankToString(rank);
      return String.format("%s-high straight flush",value);
   }

   /*Tie break given by 1) value of straight flush, then higest rank
   in the hand.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = STRAIGHT_FLUSH_VALUE;
      tieBreak[1] = cards[CARDS_PER_HAND-1].getRank();
   }
}
