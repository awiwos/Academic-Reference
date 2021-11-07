/*
Name: Andrew Woods
login ID: awwoods
Two pair abstract class to represent functions relating to two pair conept.
*/

public abstract class TwoPair{
   private final static int CARDS_PER_HAND = 5;
   private final static int TWO_PAIR_VALUE = 2;

   /*Noted not to be called if hand matches better result. Returns true if
   two pairs found. Otherwise false*/
   public static boolean isTwoPair(Card[] cards){
      int pairsFound = 0;
      for(int i=0;i<CARDS_PER_HAND-1;i++){
         for(int j=i+1;j<CARDS_PER_HAND;j++){
            if(cards[i].getRank()==cards[j].getRank()){
               pairsFound++;
               break;
            }
         }
      }
      return pairsFound==2?true:false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of two pair*/

   /*Returns string of the form Xs over Ys where X is the rank of the
   higher pair and Y is the rank of the lower pair*/
   public static String getResult(Card[] cards){
      int rank1 = rankOfHighTwo(cards), rank2 = rankOfLowTwo(cards);
      String value1 = Card.rankToString(rank1)
            ,value2 = Card.rankToString(rank2);
      return String.format("%ss over %ss",value1,value2);
   }

   /*Tie break given by 1) value of two pair, 2) rank of higher pair,
   3) rank of the lower pair, 4) rank of the remaining card.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = TWO_PAIR_VALUE;
      tieBreak[1] = rankOfHighTwo(cards);
      tieBreak[2] = rankOfLowTwo(cards);
      tieBreak[3] = rankOfOne(cards);
   }

   /*Noted given cards are sorted by rank, hands which are two pair are of the
   form: ABBCC, AABCC, or AABBC. Therefore the fourth card always has the
   rank of the higher pair*/
   private static int rankOfHighTwo(Card[] cards){
      return cards[3].getRank();
   }

   /*Noted given cards are sorted by rank, hands which are two pair are of the
   form: ABBCC, AABCC, or AABBC. Therefore the second card always has the
   rank of the lower pair*/
   private static int rankOfLowTwo(Card[] cards){
      return cards[1].getRank();
   }

   /*return rank of single card. Noted per above if the ranks of the first
   and second cards are not equal want the rank of the first card. otherwise
   if the ranks of the fourth and fifth card are not equal want the rank
   of the fifth card. Otherwise, want the rank of the third card.*/
   private static int rankOfOne(Card[] cards){
      if(cards[0].getRank()!=cards[1].getRank()){
         return cards[0].getRank();
      } else if(cards[3].getRank()!=cards[4].getRank()){
         return cards[4].getRank();
      } else {
         return cards[2].getRank();
      }
   }
}
