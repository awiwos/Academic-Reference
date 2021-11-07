/*
Name: Andrew Woods
login ID: awwoods
Pair abstract class to represent functions relating to pair conept.
*/

public abstract class Pair{
   private final static int CARDS_PER_HAND = 5;
   private final static int PAIR_VALUE = 1;

   /*Noted not to be called if hand matches better result. Returns true if
   two cards of the same rank found. Otherwise false*/
   public static boolean isPair(Card[] cards){
      int[] ranks = Card.getHandRanks(cards);
      for(int i=0;i<CARDS_PER_HAND-1;i++){
         if(ranks[i]==ranks[i+1]){
            return true;
         }
      }
      return false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of pair*/

   /*Returns string of the form Three Xs where there are two cards of rank
   X in the hand.*/
   public static String getResult(Card[] cards){
      int rank = rankOfTwo(cards);
      String value;
      value = Card.rankToString(rank);
      return String.format("Pair of %ss",value);
   }

   /*Tie break given by 1) value of pair, 2) the rank of the pair of
   cards of equal rank, 3) by the ranks of the higher remaining cards in
   decending order.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = PAIR_VALUE;
      tieBreak[1] = rankOfTwo(cards);
      tieBreak[2] = rankOfHighOne(cards);
      tieBreak[3] = rankOfMidOne(cards);
      tieBreak[4] = rankOfLowOne(cards);
   }

   /*Returns the rank of the three cards of equal rank. Noted since cards
   are sorted by rank, they can take four forms: AABCD, ABBCD, ABCCD,
   or ABCDD. If the ranks of the first and second cards are equal, want
   the rank of the first card. Otherwise, if the ranks of the second and
   third cards are equal, want the rank of the second card. Otherwise, want
   the rank of the fourth card.*/
   private static int rankOfTwo(Card[] cards){
      if(cards[0].getRank()==cards[1].getRank()){
         return cards[0].getRank();
      } else if(cards[1].getRank()==cards[2].getRank()){
         return cards[1].getRank();
      } else {
         return cards[3].getRank();
      }
   }

   /*return rank of the highest single card. Noted per above if the ranks
   of the fourth and fifth cards are equal want the rank of the
   third card. otherwise want the rank of the fifth card.*/
   private static int rankOfHighOne(Card[] cards){
      if(cards[3].getRank()==cards[4].getRank()){
         return cards[2].getRank();
      } else {
         return cards[4].getRank();
      }
   }

   /*return rank of the middle single card. Noted per above if the hand
   is of the first or second form detailed above, want the rank of the fourth
   card. Otherwise, want the rank of the second card.*/
   private static int rankOfMidOne(Card[] cards){
      if(     (cards[0].getRank()==cards[1].getRank())
            ||(cards[1].getRank()==cards[2].getRank())){
         return cards[3].getRank();
      } else {
         return cards[1].getRank();
      }
   }

   /*return rank of the lowest single card. Noted per above if the ranks
   of the first and second cards are equal want the rank of the
   third card. otherwise want the rank of the first card.*/
   private static int rankOfLowOne(Card[] cards){
      if(cards[0].getRank()==cards[1].getRank()){
         return cards[2].getRank();
      } else {
         return cards[0].getRank();
      }
   }
}
