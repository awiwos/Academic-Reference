/*
Name: Andrew Woods
login ID: awwoods
Full house abstract class to represent functions relating to full house
conept.
*/

public abstract class FullHouse{
   private final static int CARDS_PER_HAND = 5;
   private final static int FULL_HOUSE_VALUE = 6;

   /*Noted not to be called if hand matches better result. Returns true if
   three cards of one rank and two cards of another found. Otherwise false*/
   public static boolean isFullHouse(Card[] cards){
      int[] ranks = Card.getHandRanks(cards);
      /*Check has three of one rank and two of another rank*/
      //If lowest equal to second lowest and highest equal to second highest
      if((ranks[0]==ranks[1])&&(ranks[3]==ranks[4])){
         //if middle rank to either the lower ranks or higher ranks.
         if((ranks[1]==ranks[2])||(ranks[2]==ranks[3])){
            //Then hand has three of one kind and two of another
            return true;
         }
      }
      return false;
   }

   /*Noted all functions below this point not to be called unless hand
   determined to be of the form of full house*/

   /*Returns string of the form Xs full of Ys where the hand contains
   three cards of rank X and two cards of rank Y*/
   public static String getResult(Card[] cards){
      int rank1 = rankOfThree(cards), rank2 = rankOfTwo(cards);
      String value1, value2;
      value1 = Card.rankToString(rank1);
      value2 = Card.rankToString(rank2);
      return String.format("%ss full of %ss",value1,value2);
   }

   /*Tie break given by 1) value of full house, 2) the rank of the three
   cards of equal rank, then 3) by the value of the pair of cards of equal
   rank.*/
   public static void getTieBreak(int[] tieBreak, Card[] cards){
      tieBreak[0] = FULL_HOUSE_VALUE;
      tieBreak[1] = rankOfThree(cards);
      tieBreak[2] = rankOfTwo(cards);
   }

   /*Returns the rank of the three cards of equal rank. Noted since cards
   are sorted by rank, they can take two forms: AAABB or AABBB. Either way
   the rank of the third card is the rank of the three.*/
   private static int rankOfThree(Card[] cards){
      return cards[2].getRank();
   }

   /*Returns the rank of the pair. Noted per above if the ranks
   of the second and third cards are equal want to return the rank of the
   fourth card, otherwise want to return the rank of the second card.*/
   private static int rankOfTwo(Card[] cards){
      if(cards[1].getRank()==cards[2].getRank()){
         return cards[3].getRank();
      } else {
         return cards[1].getRank();
      }
   }
}
