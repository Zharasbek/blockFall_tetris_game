#ifndef PA2_LEADERBOARD_H
#define PA2_LEADERBOARD_H

#include <ctime>
#include <string>
#include "LeaderboardEntry.h"

#define MAX_LEADERBOARD_SIZE 10

using namespace std;

class Leaderboard {
public:
    LeaderboardEntry* head_leaderboard_entry = nullptr;
    Leaderboard() : head_leaderboard_entry(nullptr), insertionCount(0){};
    int count_leaderboard_entries() const; 
    void free_leaderboard_entries(LeaderboardEntry* entry); // Helper function to free the memory used for leaderboard entries
    void trim_leaderboard(); // Helper function to trim the leaderboard to the top 10 entries
    string format_timestamp(time_t timestamp); // Helper function to format the timestamp in the leaderboard file
    void read_from_file(const string &filename);
    void write_to_file(const string &filename);
    void print_leaderboard();
    void insert_new_entry(LeaderboardEntry *new_entry);
    virtual ~Leaderboard();
private:
    int insertionCount;  // Member variable to track the number of insertions
};


#endif //PA2_LEADERBOARD_H
