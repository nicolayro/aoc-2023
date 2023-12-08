#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define HAND_SIZE   5
#define MAX_HANDS   1000

enum CardType {
    JOKER = 1,
    T = 10,
    J,
    Q,
    K,
    A
};

enum HandType {
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULl_HOUSE,
    FOUR_KIND,
    FIVE_KIND
};

typedef struct {
    int cards[HAND_SIZE]; // Cards in hand
    int bid;              // Hand bid
} Hand;

int jokers;     // Flag to tell if jokers are in play (for part 2):

int num_hands;  // Number of hands parsed
Hand *hands;    // List of all hands

void parse_input(void);
int is_digit(char c);
int parse_card(char c);
int comp_hand(const void *, const void *);

int main()
{
    hands = malloc(MAX_HANDS * sizeof(Hand));

    // Part 1
    parse_input();

    qsort(hands, num_hands, sizeof(Hand), comp_hand);

    long sum1 = 0;
    for (int i = 0; i < num_hands; ++i) {
        sum1 += (num_hands - i) * hands[i].bid;
    }

    // Part 2
    num_hands = 0;
    jokers = 1;

    parse_input();

    qsort(hands, num_hands, sizeof(Hand), comp_hand);

    long sum2 = 0;
    for (int i = 0; i < num_hands; ++i) {
        sum2 += (num_hands - i) * hands[i].bid;
    }

    printf("%ld\n", sum1);
    printf("%ld\n", sum2);

    free(hands);

    return 0;
}

void parse_input()
{
    char line[64];
    char *filename = "input.txt";
    FILE *file = fopen(filename, "r");

    while (fgets(line, sizeof(line), file)) {
        for (int i = 0; i < HAND_SIZE; ++i)
            hands[num_hands].cards[i] = parse_card(line[i]);

        hands[num_hands].bid = strtol(line + HAND_SIZE, NULL, 10);
        num_hands++;
    }

    fclose(file);
}

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int parse_card(char c)
{
    if (is_digit(c)) {
        return c - '0';
    } else {
        switch (c) {
            case 'T':
                return T;
            case 'J':
                return jokers ? 1 : J;
            case 'Q':
                return Q;
            case 'K':
                return K;
            case 'A':
                return A;
            default:
                assert(0 && "Unhandled char");
        }
    }
}

int evaluate(Hand *h)
{
    int counter = 0;
    int total_matches = 0;
    int highest_match = 0;

    for (int i = 0; i < HAND_SIZE - 1; ++i) {
        counter = 0;
        for (int j = i + 1; j < HAND_SIZE; ++j) {
            if (h->cards[i] == h->cards[j]) {
                counter++;
            }
        }
        highest_match = counter > highest_match ? counter : highest_match;
        total_matches += counter;
    }

    if (highest_match == 4)
        return FIVE_KIND;
    if (highest_match == 3)
        return FOUR_KIND;
    if (highest_match == 2 && total_matches == 4)
        return FULl_HOUSE;
    if (highest_match == 2)
        return THREE_KIND;
    if (highest_match == 1 && total_matches == 2)
        return TWO_PAIR;
    if (highest_match == 1)
        return ONE_PAIR;

    return HIGH_CARD;
}

int evaluate_with_joker(Hand *h)
{
    int counter;
    int total_matches = 0;
    int highest_match = 0;
    int highest_match_card = h->cards[HAND_SIZE - 1]; // Assume last card

    // Find highest matching, non-joker card
    for (int i = 0; i < HAND_SIZE - 1; ++i) {
        if (h->cards[i] == JOKER) {
            continue;
        }

        counter = 0;
        for (int j = i + 1; j < HAND_SIZE; ++j) {
            if (h->cards[i] == h->cards[j]) {
                counter++;
            }
        }

        if (highest_match_card == JOKER || counter > highest_match) {
            highest_match = counter;
            highest_match_card = h->cards[i];
        }
    }

    // Evaluate, but treat joker as highest matching card
    int card_a, card_b;
    for (int i = 0; i < HAND_SIZE - 1; ++i) {
        counter = 0;
        card_a = h->cards[i] == JOKER ? highest_match_card : h->cards[i];
        for (int j = i + 1; j < HAND_SIZE; ++j) {
            card_b = h->cards[j] == JOKER ? highest_match_card : h->cards[j];
            if (card_a == card_b) {
                counter++;
            }
        }
        highest_match = counter > highest_match ? counter : highest_match;
        total_matches += counter;
    }

    if (highest_match == 4)
        return FIVE_KIND;
    if (highest_match == 3)
        return FOUR_KIND;
    if (highest_match == 2 && total_matches == 4)
        return FULl_HOUSE;
    if (highest_match == 2)
        return THREE_KIND;
    if (highest_match == 1 && total_matches == 2)
        return TWO_PAIR;
    if (highest_match == 1)
        return ONE_PAIR;

    return HIGH_CARD;
}

int comp_hand(const void *a, const void *b)
{
    Hand *ha = (Hand *) a;
    Hand *hb = (Hand *) b;

    int ea, eb;
    if (jokers) {
        ea = evaluate_with_joker(ha);
        eb = evaluate_with_joker(hb);
    } else {
        ea = evaluate(ha);
        eb = evaluate(hb);
    }

    if (ea != eb)
        return eb - ea;

    for (int i = 0; i < HAND_SIZE; ++i) {
        if (ha->cards[i] != hb->cards[i])
            return hb->cards[i] - ha->cards[i];
    }

    return 0;
}
