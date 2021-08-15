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
}
pair;

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
bool check_cycle(int w, int l);

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
    
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name,  candidates[i]) == 0)
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

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = candidate_count * (candidate_count - 1) / 2;
    int w = 0;
    
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i; j < pair_count; j++)
        {
            //Ignores the diagonal
            if (i == j)
            {
                continue;
            }
            //If it is a tie dont add
            else if (preferences[i][j] == preferences[j][i])
            {
                continue;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[w].winner = i;
                pairs[w].loser = j;
                w ++;
            }
            else
            {
                pairs[w].winner = j;
                pairs[w].loser = i;
                w ++;
            }

        }
        
    }
    //Final value of pair_count
    pair_count = w;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Apply selection sorting
    for (int i = 0; i < pair_count; i++)
    {
        int max_idx = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[max_idx].winner][pairs[max_idx].loser])
            {
                max_idx = j;
                pair aux1 = pairs[max_idx];
                pairs[max_idx] = pairs[i];
                pairs[i] = aux1;
                
            }
        }    
    }
    
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    
    
    for (int i = 0; i < pair_count; i++)
    {   
        //Use function to lock
        locked[pairs[i].winner][pairs[i].loser] = check_cycle(pairs[i].winner, pairs[i].loser);
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    
    for (int i = 0; i < candidate_count; i++)
    {
        int count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            //Ignore diagonal
            if (j == i)
            {
                continue;
            }
            else
            {
                //Checks what collum is all false
                if (locked[j][i])
                {
                    count ++;
                }
                else
                {
                    continue;
                }
            }
                
        }
        if (count == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}

bool check_cycle(int w, int l)
{
    //if the last candidate equals the first it forms a cycle
    if (l == w)
    {
        return false;    
    }
    
    for (int i = 0; i < pair_count; i++)
    {
        //If it is a locked pair check where it leads
        if (locked[l][i] == true)
        {
            return check_cycle(w, i);
        }
    }
    return true;
}
