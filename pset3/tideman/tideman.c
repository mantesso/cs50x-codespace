#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int pair_margin(pair p);
bool detect_cycle(int winner, int loser);
bool check_column(int col_number);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    // (mine) print preferences array for debugging
    // printf("preferences array:\n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i", preferences[i][j]);
    //     }
    //     printf("\n");
    // }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count - 1; j++)
        {
            preferences[ranks[i]][ranks[j + 1]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
        }
    }
    // printf("pair_count is %i\n", pair_count);

    return;
}

// returns margin for a given pair of candidates
int pair_margin(pair p)
{
    return preferences[p.winner][p.loser] - preferences[p.loser][p.winner];
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // printf current unsorted pairs array
    // for (int i = 0; i < pair_count; i++)
    // {
    // printf("pairs[%i] winner: %i, loser: %i\n", i, pairs[i].winner, pairs[i].loser);
    // margin for current pair
    // int margin = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    // int margin = pair_margin(pairs[i]);
    // printf("margin: %i\n", margin);
    // }

    pair temp;
    bool swapped;

    for (int i = 0; i < pair_count - 1; i++)
    {
        swapped = false;
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (pair_margin(pairs[j]) < pair_margin(pairs[j + 1]))
            {
                temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }

    // print sorted array
    //  printf("\nsorted pairs array\n");
    //  for (int i = 0; i < pair_count; i++)
    //  {
    //  printf("%i, w:%i, l:%i margin: %i\n", i, pairs[i].winner, pairs[i].loser, pair_margin(pairs[i]));
    // }
    // printf("\n");

    return;
}

// prints locked array
void print_locked(void)
{
    printf("locked:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        bool cycle = detect_cycle(pairs[i].winner, pairs[i].loser);
        if (cycle == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    // print_locked();
    return;
}

// a path exists from loser to winner?
// only check already locked pairs
//  if detect_cycle returns true, the added pair creates a cycle
bool detect_cycle(int winner, int loser)
{
    if (locked[loser][winner])
    {
        return true;
    }

    for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser] == true && pairs[i].winner == loser)
        {
            if (detect_cycle(winner, pairs[i].loser) == true)
            {
                return true;
            }
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (check_column(i))
        {
            // printf("winner is: ");
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

// returns true if every line of a column is false
bool check_column(int col_number)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][col_number])
        {
            return false;
        }
    }
    return true;
}