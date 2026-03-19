#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// ── Limits ────────────────────────────────────────────────────────────────────
#define MAX_VOTERS     100
#define MAX_CANDIDATES   9

// ── Data ──────────────────────────────────────────────────────────────────────
// preferences[i][j] = the index (in candidates[]) of voter i's j-th ranked choice
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Each candidate has a name, current round vote total, and an elimination flag
typedef struct
{
    string name;
    int    votes;
    bool   eliminated;
} candidate;

candidate candidates[MAX_CANDIDATES];

// Global counts set in main()
int voter_count;
int candidate_count;

// ── Function prototypes — do NOT change these signatures ──────────────────────
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int  find_min(void);
bool is_tie(int min);
void eliminate(int min);

// ── main() — provided, do not modify ─────────────────────────────────────────
int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name       = argv[i + 1];
        candidates[i].votes      = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Collect each voter's full ranking
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // Run rounds until we have a winner or a tie
    while (true)
    {
        tabulate();

        bool won = print_winner();
        if (won)
        {
            break;
        }

        int  min = find_min();
        bool tie = is_tie(min);

        if (tie)
        {
            // All remaining candidates tie — print them all
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        eliminate(min);

        // Reset vote counts for next round
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank]= i;
            return true;
        }
    }
    return false;
}

void tabulate(void)
{
    for(int i = 0; i < voter_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            int c = preferences[i][j];
            if(candidates[c].eliminated == false)
            {
                candidates[c].votes++;
                break;
            }
        }
    }
}

bool print_winner(void)
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(!candidates[i].eliminated &&
            candidates[i].votes > voter_count / 2)
            {
                printf("%s\n", candidates[i].name);
                return true;
            }
    }
    return false;
}

int find_min(void)
{
    int min = voter_count;

    for(int i = 0; i < candidate_count; i++)
    {
        if(!candidates[i].eliminated)
        {
            if(candidates[i].votes < min)
            {
                min = candidates[i].votes;
            }
        }
    }
    return min;
}

bool is_tie(int min)
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(!candidates[i].eliminated)
        {
            if(candidates[i].votes != min)
            {
                return false;
            }
        }
    }
    return true;
}

void eliminate(int min)
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}
