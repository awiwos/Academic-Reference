/*
Name: Andrew Woods
login ID: awwoods
Three of a kind abstract class to represent functions relating to three of a
kind conept.
*/

public abstract class ThreeOfAKind{
   private final static int CARDS_PER_HAND = 5;
   private final static int THREE_OF_A_KIND_VALUE = 3;

   /*Noted not to be called if hand matches better result. Returns true if
   three cards of a signle rank found. Otherwise false*/
   public static boolean isThreeOfAKind(Card[] cards){
      int[] ranks = Card.getHandRanks(cards);
      for(int i=0;i<3;i++){
         if((ranks[i]==ranks[i+1])&&(ranks[i]==ranks[i+2])){
            return true;
         }
      }
      return false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of three of a kind*/

   /*Returns string of the form Three Xs where there are three cards of rank
   X in the hand.*/
   public static String getResult(Card[] cards){
      int rank = -1;
      rank = rankOfThree(cards);
      String value = Card.rankToString(rank);
      return String.format("Three %ss",value);
   }

   /*Tie break given by 1) value of three of a kind, 2) the rank of the three
   cards of equal rank, 3) by the rank of the higher remaining card, then
   4) the rank of the lower remaining card.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = THREE_OF_A_KIND_VALUE;
      tieBreak[1] = rankOfThree(cards);
      tieBreak[2] = rankOfHighOne(cards);
      tieBreak[3] = rankOfLowOne(cards);
   }

   /*Returns the rank of the three cards of equal rank. Noted since cards
   are sorted by rank, they can take three forms: AAABC, ABBBC, or ABCCC.
   Either way the rank of the third card is the rank of the three.*/
   private static int rankOfThree(Card[] cards){
      return cards[2].getRank();
   }

   /*return rank of the higher single card. Noted per above if the ranks
   of the second and third cards are not equal want the rank of the
   second card. otherwise want the rank of the fifth card.*/
   private static int rankOfHighOne(Card[] cards){
      if(cards[1].getRank()!=cards[2].getRank()){
         return cards[1].getRank();
      } else {
         return cards[4].getRank();
      }
   }

   /*return rank of the higher single card. Noted per above if the ranks
   of the third and fourth cards are not equal want the rank of the
   fourth card. otherwise want the rank of the first card.*/
   private static int rankOfLowOne(Card[] cards){
      if(cards[2].getRank()!=cards[3].getRank()){
         return cards[3].getRank();
      } else {
         return cards[0].getRank();
      }
   }
}
