#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT_SET(v, n) ( v |= BIT(n) )


#include "omaha_poker.h"
struct player player1;
struct player player2;
/* TODO: TASK 1 (1 point) : Define 2 global instances of struct player--one for each player */

/* Parse card from a 2 char input */
struct card parse(const char *card)
{
	 /* return parse_ref(card);*/
  if(card == NULL) return;
  
  struct card toRet;
    char v = card[0];
    switch(v)
    {
    case '1':
        toRet.val = ONE; break;
    case '2':
        toRet.val = TWO; break;
    case '3':
        toRet.val = THREE; break;
    case '4':
        toRet.val = FOUR; break;
    case '5':
        toRet.val = FIVE; break;
    case '6':
        toRet.val = SIX; break;
    case '7':
        toRet.val = SEVEN; break;
    case '8':
        toRet.val = EIGHT; break;
    case '9':
        toRet.val = NINE; break;
    case 'T':
        toRet.val = TEN; break;
    case 'J':
        toRet.val = JACK; break;
    case 'Q':
        toRet.val = QUEEN; break;
    case 'K':
        toRet.val = KING; break;
    case 'A':
        toRet.val = ACE; break;
    }

    char s = card[1];
  
    switch (s)
    {
    case 'C':
        toRet.suit = CLUB; break;
    case 'D':
        toRet.suit = DIAMOND; break;
    case 'S':
        toRet.suit = SPADE; break;
    case 'H':
        toRet.suit = HEART; break;
    }
    return toRet;
  
}
	/* TODO: TASK 2 (10 points): Implement this function. 
	 * The function accepts a 2-character array called card (e.g., 'AH')
	 * Create an instance of struct card and populate it with the correct enum values based on the input chars value and suit.
	 * return the card */
	/* Hint: Use a switch statement to get the value from card[0] and another switch statement to get suit from card[1] */


/* Count the number of occurrences of each card 2 through Ace */
void count_cards(struct hand *h)
{
  /* return count_cards(h);*/ 
  if(h == NULL) return;
  unsigned int (*p) =  h->card_count;
  struct card *c = h->cards;
  int i;
  for(i = 0; i < 5; i++)
    {
      unsigned int val = c->val -2;
      c++;
      *(p + val) += 1;
    }	
	/* TODO: TASK 3 (6 points): Implement this function
	 * The function accepts a pointer to a hand that has its cards already setup.
	 * This function must iterate through the cards in the hand and count the number of times
	 * a card appears, and update the counter for that card */
}

int is_flush(struct hand *h)
{
 /* return is_flush_ref(h); */
  if(h==NULL) return;
  struct card *c = h->cards;
  unsigned int Fsuit = c->suit;
  int i;
  for(int i = 1; i < 5; i++)
    {
        c++;
        unsigned int testVal = c->suit;
        if(testVal != Fsuit) {return 0;}
    }
  return 1;

	/* TODO: TASK 4 (6 points): Implement this function.
	 * Return 1 if suits of all 5 cards in the hand are the same, else return 0 */
}

int is_straight(struct hand *h)
{
  /* return is_straight_ref(h); */

  if(h == NULL) return;
    unsigned int *p = h->card_count;
    int i,j, sequence;

    for(i = 0; i < 9; i++)
    {
        if(*(p+i) == 1) /* Finds First Card of Sequence */
        {
            sequence = 1;
            for(j = i+1; j < i + 5; j++)
            {
                if(*(p+j) != 1)     /* Checks next 4 cards, if any of them are not 1, it is not a straight */
                {
                    sequence = 0;
                    break;
                }
            }
            break;
        }
    }
    if(sequence == 1) {return 1;}

    /* check 5 high straight */
    if(*(p+12)!=1) /* If there is no ACE */
    {
        return 0;
    }
    sequence = 1;
    for(i = 0; i < 4; i++)
    {
        if(*(p+i) != 1)
        {
            sequence = 0;
        }
    }
    return sequence;


	/* TODO: TASK 5 (10 points): Implement this function. 
	 * Return 1 if the 5 cards in the hand form a sequence, else return 0
	   NOTE: You must handle the exception for A2345, where A is not 1 but 13 */

	/* HINT: The card_count in the hand structure contains the number of times 
	 * each card value appears in the hand. See if 5 consecutive cards have a card_count value of 1 */
}

int is_straight_flush(struct hand *h)
{
/* return is_straight_flush_ref(h); */
  if(h==NULL) return;
	int x = is_straight(h);
	int y = is_flush(h);
	if(x && y) {return 1;}
	return 0;
  
}
		

/* This function prints a hand. Useful in debugging */
void print_hand(struct hand *h)
{
	int i;
	for(i = 0; i < 5; i++) {
		printf("%d%d ", h->cards[i].val, h->cards[i].suit);
	}
}


/* This is the main function that converts the player's hand into weighted unsigned long number. 
It is a 55bit vector as shown below (2 is the LSB and StraightFlush is the MSB) */
/* 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A, 22, 33, 44, 55, 66, 77, 88, 99, TT, JJ, QQ, KK, AA,
222, 333, 444, 555, 666, 777, 888, 999, TTT, JJJ, QQQ, KKK, AAA, Straight, Flush, FullHouse, 2222, 3333, 
4444, 5555, 6666, 7777, 8888, 9999, TTTT, JJJJ, QQQQ, KKKK, AAAA, StraightFlush */
/* The number of occurrences of each number in the hand is first calculated in count_cards. 
Then, depending on the numeber of occurrences, the corresponding bit in the bit-vector is set. 
In order to find the winner, a simple comparison of the bit vectors (i.e., unsigned long integers) will suffice! */
void eval_strength(struct hand *h)
{
  /* return eval_strength_ref(h); */
  
  if(h==NULL) return;
  
  count_cards(h);
  
  unsigned long v = 0;
 
  unsigned int l = 0;
  unsigned int hasPair, hasTrips;
  hasPair = hasTrips = 0;
  unsigned int *c = h->card_count;

  int i;
  for(int i = 0; i < 13; i++)    /* LOOPS THROUGH FOR SINGLE CARDS */
    {
      if(*(c+i) == 1){ BIT_SET(v, l);}
      l++;
    }

  for(int i = 0; i < 13; i++)    /* LOOPS THROUGH FOR PAIRS */
    {
      if(*(c+i) == 2) {BIT_SET(v, l); hasPair = 1; }
      l++;
    }
  
  for(int i = 0; i < 13; i++)    /* LOOPS THROUGH FOR TRIPS */
    {
      if(*(c+i) == 3) {BIT_SET(v, l); hasTrips = 1; }
      l++;
    }
  
  if(is_straight(h)) {BIT_SET(v, l);}
  l++;

  if(is_flush(h)) {BIT_SET(v, l);}
  l++;
  
  if(hasPair && hasTrips) {BIT_SET(v, l);}
  l++;

  for(int i = 0; i < 13; i++)    /* LOOPS THROUGH FOR QUADS */
    {
      if(*(c+i) == 4) {BIT_SET(v, l); }
      l++;
    }

  if(is_straight_flush(h)) {BIT_SET(v, l);}
  

  
  h->vector = v;
  

	/* TODO: TASK 7 (25 points): Implement this function.
	 * Given a hand, iterate through the cards and use the BIT macros to set the appropriate bit in the hand vector */
}


void eval_players_best_hand(struct player *p)
{
	//return eval_players_best_hand_ref(p); 
	/* TODO: TASK 8 (10 points): Implement this function. 
	 * For each possible hand the player can make, evaluate the strength of the hand (by calling eval_strength).
	 * Then, set the best_hand vector for the player to point to the strongest hand.
	 */
	if(p == NULL) return;
	struct hand *current_best = p->hands; // first hand starts as current_best hand
	for(int i = 1; i < MAX_COMBINATIONS; i++){
		eval_strength(p->hands + i);
		if(p->hands[i].vector > current_best->vector) current_best = p->hands + i; // adds 8*i bytes to address of hand array
	}
	p->best_hand = current_best;
		
}

void copy_card(struct card *dst, struct card *src)
{
	//return copy_card_ref(dst, src); 
	/* TODO: TASK 9 (3 points): Implement this function. 
	 * copy the value and suit from the src card to the dst card. 
	 */
	if(dst == NULL || src == NULL) return;
	dst->val = src->val;
	dst->suit = src->suit;
}


void initialize_player_omaha(struct player *p, struct card *player_cards, struct card *community_cards)
{
	//return initialize_player_omaha_ref(p, player_cards, community_cards);

	/* TODO: TASK 10 (25 points): Given the player cards and the community cards, initialize the array of hands in the player structure. 
	 * There are a total of MAX_COMBINATIONS number of possible hands that the player can make.
	 * Initialize each of the MAX_COMBINATIONS number of hands with different 5 card combinations. 
	 * Use the copy_card function.
	 */
	/* HINT: Start by zeroing out the player instance. Then, copy each unique combination into a hand in the hands array. 
	 * For example: P1 P2 C1 C2 C3 is one possible combination (i.e., first 2 cards of the player and first 3 cards from community cards). 
	 * So, copy player_cards[0] into hands[0].cards[0], player_cards[1] into hands[0].cards[1], community_cards[0] into hands[0].cards[2], 
	 * community_cards[1] into hands[0].cards[3] and community_cards[2] into hands[0].cards[4]. Repeat this process for each possible combination
	 * such as P1 P2 C1 C2 C4, P3 P4 C1 C2 C3, etc. 
	 * Implement helper functions as necessary. */
	if(p == NULL || player_cards == NULL || community_cards == NULL) return;
	memset(p->hands, 0, sizeof(struct hand) * MAX_COMBINATIONS);
	int h = 0;
	for(int i = 0; i<3; i++){
		for(int j = i+1; j<4; j++){
			for(int k = 0; k<3; k++){
				for(int l = k+1; l<4; l++){
					for(int m = l+1; m<5; m++){
						copy_card(p->hands[h].cards,     player_cards    + i);
						copy_card(p->hands[h].cards + 1, player_cards    + j);
						copy_card(p->hands[h].cards + 2, community_cards + k);
						copy_card(p->hands[h].cards + 3, community_cards + l);
						copy_card(p->hands[h].cards + 4, community_cards + m);
						h++;
					}
				}
			}
		}
	}	
}

void extra_credit(struct hand *h){
	if(h == NULL) return;
	for(int i = 0; i < 5; i++){
		switch(h->cards[i].val){
		case 10: printf("T"); break;
			case 11: printf("J"); break;
			case 12: printf("Q"); break;
			case 13: printf("K"); break;
			case 14: printf("A"); break;
			default: printf("%d", h->cards[i].val);
		}
		switch(h->cards[i].suit){
			case 0: printf("C"); break;
			case 1: printf("D"); break;
			case 2: printf("H"); break;
			case 3: printf("S"); break;
		}
		printf(" ");
	}
}

/* Parse each hand in the input file, evaluate the strengths of hands and identify a winner by comparing the weighted vectors */
void process_input_omaha(FILE *fp)
{
	//return process_input_omaha_ref(fp); 

	char p1[4][3];
	char p2[4][3];
	char comm[5][3];
	struct card p1_cards[4], p2_cards[4], community_cards[5];
	int i;

	while(fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s", 
		&p1[0][0], &p1[1][0], &p1[2][0], &p1[3][0], &p2[0][0], &p2[1][0], &p2[2][0], &p2[3][0], 
		&comm[0][0], &comm[1][0], &comm[2][0], &comm[3][0], &comm[4][0]) == 13) {

		memset(p1_cards, 0, sizeof(struct card) * 4);
		memset(p2_cards, 0, sizeof(struct card) * 4); 
		memset(community_cards, 0, sizeof(struct card) * 5);

		for(i = 0; i < 4; i++) {
			p1_cards[i] = parse(&p1[i][0]);
			p2_cards[i] = parse(&p2[i][0]);
		}

		for(i = 0; i < 5; i++) 
			community_cards[i] = parse(&comm[i][0]);

		/* TODO: TASK 11 (1 point): Initialize each player with p1_cards and community_cards by calling initialize_player_omaha */
		struct player *player1 = NULL;
		struct player *player2 = NULL;
		player1 = (struct player*) malloc(sizeof(struct player));
		if(player1 == NULL) return;
		player2 = (struct player*) malloc(sizeof(struct player));
		if(player2 == NULL){
			free(player1);
			return;
		}
		initialize_player_omaha(player1, p1_cards, community_cards);
		initialize_player_omaha(player2, p2_cards, community_cards);
		
		/* TODO: TASK 12 (1 point): Evaluate the best hand for a player by calling eval_players_best_hand */
		eval_players_best_hand(player1);
		eval_players_best_hand(player2);

		/* TODO: TASK 13 (1 point): Depending on who has the stronger hand, print the winner. 
		 * If both have the same strong hand, print "No single winner" */
		if(player1->best_hand->vector > player2->best_hand->vector) printf("Player 1 wins\n");
		else if(player1->best_hand->vector < player2->best_hand->vector) printf("Player 2 wins\n");
		else if(player1->best_hand->vector == player2->best_hand->vector) printf("No single winner\n");
		
		//extra credit
		printf("Player 1's hand: ");
		extra_credit(player1->best_hand);
		printf("\nPlayer 2's hand: ");		
		extra_credit(player2->best_hand);		
		printf("\n************************\n");

		free(player1);
		free(player2);		
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;

	if(argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
		printf("Unable to open input file\n");
		exit(-1);
	}
	
	process_input_omaha(fp);

	return 0;
}
	
