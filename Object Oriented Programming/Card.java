/*
Name: Andrew Woods
login ID: awwoods
Card Class to represent cards and card related functions.
*/

public class Card{
   private int rank;
   private int suit;
   private final static int ERROR = -1;
   private final static int CARDS_PER_HAND = 5;
   private final static int CARD_RANK_OFFSET = 2;
   private final String STRING_O_RANKS = "23456789TJQKA";
   private final String STRING_O_SUITS = "HDSC";

   /*Constructr: takes all caps string representing card. Turns first
   character into the card's rank, and the second into the suit.
   Errors out if invalid card.*/

   Card(String input){
      //Set Assign rank value based on location in ranks string
      if(STRING_O_RANKS.indexOf(input.charAt(0))==ERROR){//catch invalid card
         System.out.printf("Error: invalid card name '%s'\n",input);
         System.exit(ERROR);
      }
      this.rank = STRING_O_RANKS.indexOf(input.charAt(0));

      //Set Assign suit value based on location in suits string
      if(STRING_O_SUITS.indexOf(input.charAt(1))==ERROR){//catch invalid card
         System.out.printf("Error: invalid card name '%s'\n",input);
         System.exit(ERROR);
      }
      this.suit = STRING_O_SUITS.indexOf(input.charAt(1));
   }

   /*Getter: returns integer representation of the suit of the card*/
   public int getSuit(){
      return this.suit;
   }

   /*Getter: returns integer representation of the rank of the card*/
   public int getRank(){
      return this.rank;
   }

   /*sort cards by lowest to highest ranks by insertion sort*/
   static void sortCards(Card[] cards){
      for(int i=1;i<CARDS_PER_HAND;i++){
         for(int j=i;(j>0)&&(cmp(cards[j-1],cards[j]));j--){
            cardSwap(cards,j);
         }
      }
   }

   /*Comparison: returns true if the rank of isGreater is greater than than is
   otherwise false.*/
   static private boolean cmp(Card isGreater, Card than){
      if(isGreater.getRank()>than.getRank()){
         return true;
      }
      return false;
   }

   /*Swap card at position j with card at position j-1*/
   static private void cardSwap(Card[] cards, int j){
      Card tmp = cards[j];
      cards[j] = cards[j-1];
      cards[j-1] = tmp;
   }

   /*Returns string value associated with rank of card.*/
   static public String rankToString(int rank){
      if(rank<8){
         //Noted cards with rank 2 through 9 are stored with values 0 through 7
         return String.format("%d",rank+CARD_RANK_OFFSET);
      }
      switch(rank){
         case 8:
            return "10";
         case 9:
            return "Jack";
         case 10:
            return "Queen";
         case 11:
            return "King";
         case 12:
            return "Ace";
      }
      System.exit(ERROR);
      return "ERROR";
   }

   /*Returns an array of five integers representing the ranks of cards for
   a given hand.*/
   static public int[] getHandRanks(Card[] cards){
      int[] ranks = new int[5];
      for(int i=0;i<CARDS_PER_HAND;i++){
         ranks[i] = cards[i].getRank();
      }
      return ranks;
   }
}
