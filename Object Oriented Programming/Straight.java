/*
Name: Andrew Woods
login ID: awwoods
Straight abstract class to represent functions relating to straight conept.
*/

public abstract class Straight{
   private final static int CARDS_PER_HAND = 5;
   private final static int STRAIGHT_VALUE = 4;

   /*Returns true if card ranks are sequential. Otherwise false*/
   public static boolean isStraight(Card[] cards){
      for(int i=0;i<CARDS_PER_HAND-1;i++){
         if(cards[i+1].getRank()!=cards[i].getRank()+1){
            return false;
         }
      }
      return true;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of a straight*/

   /*Returns string of the form Xs-high straight where X is the highest
   rank in the hand*/
   public static String getResult(Card[] cards){
      int rank = cards[CARDS_PER_HAND-1].getRank();
      String value = Card.rankToString(rank);
      return String.format("%s-high straight",value);
   }

   /*Tie break given by 1) value of straight, then higest rank
   in the hand.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = STRAIGHT_VALUE;
      tieBreak[1] = cards[CARDS_PER_HAND-1].getRank();
   }
}
