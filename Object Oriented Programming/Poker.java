/*
Program to evaluate poker hands of 5 cards. Example command line run:
java Poker AH AS AC AD TC KH KS KC KD TD
should evaluate to player 1 wins with four of aces over four of kings.
*/

public class Poker{
   public final static int CARDS_PER_HAND = 5;

   /*Main control function for program.*/
   public static void main(String[] args){
      int players;
      Hand[] hands = new Hand[100];

      //Error checking: error out if args are not a positive multiple of five.
      if((args.length%5!=0)||args.length==0){
         System.out.print("Error: wrong number of arguments");
         System.out.print("; must be a multiple of 5\n");
         System.exit(-1);
      }

      //calculate the number of players. Then for each player, instantiate
      //their hand, and output the result.
      players = args.length/CARDS_PER_HAND;
      for(int i=0; i<(players);i++){
         hands[i] = new Hand(i+1,args,CARDS_PER_HAND*i,CARDS_PER_HAND*(i+1));
         System.out.printf("Player %d: %s\n",i+1,hands[i].getResult());
      }

      //If there's more than one player, print winner
      if(players>1){
         printWinner(hands, players);
      }

   }

   /*Print the winning player(s)*/
   private static void printWinner(Hand[] hands, int players){
      int winners;

      //sort hands by best to worst, and count how many players have the
      //best hand
      insSortHands(hands, players);
      winners = countWinners(hands, players);

      //print out the winner if one winning hand, or winners if multiple.
      if(winners==1){
         System.out.printf("Player %d wins.\n",hands[0].getID());
      } else {
         System.out.printf("Players %d",hands[0].getID());
         for(int i=1;i<winners-1;i++){
            System.out.printf(", %d",hands[i].getID());
         }
         System.out.printf(" and %d draw.\n",hands[winners-1].getID());
      }
   }

   /*Insertion sort to sort hands of players by best to worst.*/
   private static void insSortHands(Hand[] hands, int players){
      for(int i=0;i<players;i++){
         for(int j = i;j>0&&(cmp(hands[j-1],hands[j]));j--){
            swap(hands,j-1,j);
         }
      }
   }

   /*Comparison for insertion sort, returns true if isWorse is worse than
   than is*/
   private static boolean cmp(Hand isWorse, Hand than){
      for(int i=0;i<6;i++){
         if(isWorse.getTieBreak()[i]<than.getTieBreak()[i]){
            return true;
         } else if(isWorse.getTieBreak()[i]>than.getTieBreak()[i]){
            return false;
         }
      }
      return false;
   }

   /*Swaps elements first and second in array of hands.*/
   private static void swap(Hand[] hands, int first, int second){
      Hand tmp = hands[first];
      hands[first] = hands[second];
      hands[second] = tmp;
   }

   /*Given array of hands sorted best to worst, counts how many hands
   are equivalent to the best hand.*/
   private static int countWinners(Hand[] hands, int players){
      int winners = 1;
      for(int i=1;i<players;i++){
         if(isEqualHand(hands[0],hands[i])){
            winners++;
         } else {
            break;
         }
      }
      return winners;
   }

   /*Returns true is the value of two hands is equal i.e. they have the
   same tie break values.*/
   private static boolean isEqualHand(Hand is, Hand to){
      boolean isEqual = true;
      for(int i=0;i<6;i++){
         if(is.getTieBreak()[i]!=to.getTieBreak()[i]){
            isEqual = false;
            break;
         }
      }
      return isEqual;
   }
}
