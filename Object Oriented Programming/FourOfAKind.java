/*
Name: Andrew Woods
login ID: awwoods
Four of a kind abstract class to represent functions relating to four of a
kind conept.
*/

public abstract class FourOfAKind{
   private final static int CARDS_PER_HAND = 5;
   private final static int FOUR_OF_A_KIND_VALUE = 7;

   /*Noted not to be called if hand matches better result. Returns true if
   four cards of one rank found. Otherwise false*/
   public static boolean isFourOfAKind(Card[] cards){
      //4 of a kind of the forms AAAAB or ABBBB therefore true is first rank
      //is equal to forth or second rank is equal to fifth
      int[] ranks = Card.getHandRanks(cards);
      if((ranks[0]==ranks[3])||ranks[1]==ranks[4]){
         return true;
      }
      return false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of four of a kind*/

   /*Returns string of the form Four Xs where hand contains four cards of
   rank X*/
   public static String getResult(Card[] cards){
      int rank = rankOfFour(cards);
      String value = Card.rankToString(rank);
      return String.format("Four %ss",value);
   }

   /*Tie break given by 1) value of four of a kind, 2) by the rank of
   the four cards of equal rank, and 3) by the rank of the remaining card.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = FOUR_OF_A_KIND_VALUE;
      tieBreak[1] = rankOfFour(cards);
      tieBreak[2] = rankOfOne(cards);
   }

   /*Returns the rank of the four cards of equal rank. Noted since cards
   are sorted by rank, they can take two forms: AAAAB or ABBBB. Either way
   the rank of the second card is the rank of the four.*/
   private static int rankOfFour(Card[] cards){
      return cards[1].getRank();
   }

   /*Returns the rank of the remaining card. Noted per above if the ranks
   of the first and second cards are equal want to return the rank of the
   fifth card, otherwise want to return the rank of the first card.*/
   private static int rankOfOne(Card[] cards){
      if(cards[0].getRank()==cards[1].getRank()){
         return cards[CARDS_PER_HAND-1].getRank();
      } else {
         return cards[0].getRank();
      }
   }
}
