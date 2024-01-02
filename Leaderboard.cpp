#include "Leaderboard.h"
#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <iomanip>  // for std::setw


void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
    insertionCount++;
    if (!head_leaderboard_entry) {
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* current = head_leaderboard_entry;
        LeaderboardEntry* previous = nullptr;

        while (current && new_entry->score <= current->score) {
            previous = current;
            current = current->next_leaderboard_entry;
        }

        if (previous) {
            previous->next_leaderboard_entry = new_entry;
        } else {
            head_leaderboard_entry = new_entry;
        }
        new_entry->next_leaderboard_entry = current;
    }
    if (insertionCount>10){
        trim_leaderboard();

    }
}

void Leaderboard::trim_leaderboard() {
    
    
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* previous = nullptr;

    do {
        previous = current;
        current = current->next_leaderboard_entry;
    } while (current->next_leaderboard_entry != nullptr);
    
    previous->next_leaderboard_entry = nullptr;
    free_leaderboard_entries(current);
    
    
}

int Leaderboard::count_leaderboard_entries() const {
    int count = 0;
    LeaderboardEntry* current = head_leaderboard_entry;

    while (current) {
        count++;
        current = current->next_leaderboard_entry;
    }

    return count;
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    std::ofstream file(filename);

    if (file.is_open()) {
        LeaderboardEntry* current = head_leaderboard_entry;

        while (current) {
            file << current->score << " "
                 << current->last_played << " "
                 << current->player_name << "\n";
            current = current->next_leaderboard_entry;
        }

        file.close();
    }
}

// Helper function to free the memory used for leaderboard entries
void Leaderboard::free_leaderboard_entries(LeaderboardEntry* entry) {
    while (entry) {
        LeaderboardEntry* temp = entry;
        entry = entry->next_leaderboard_entry;
        delete temp;
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    // Clear existing leaderboard entries
    free_leaderboard_entries(head_leaderboard_entry);
    head_leaderboard_entry = nullptr;

    std::ifstream file(filename);
    if (file.is_open()) {
        unsigned long score;
        time_t lastPlayed;
        string playerName;

        while (file >> score >> lastPlayed >> playerName) {
            LeaderboardEntry* new_entry = new LeaderboardEntry(score, lastPlayed, playerName);
            insert_new_entry(new_entry);
        }

        file.close();
    }
    
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    LeaderboardEntry* current = head_leaderboard_entry;
    int order = 1;

    cerr << "Leaderboard\n-----------\n";

    while (current) {
        cerr << order << ". " << current->player_name << " "
                  << current->score << " "
                  << format_timestamp(current->last_played) << "\n";
        current = current->next_leaderboard_entry;
        order++;
    }

}

string Leaderboard::format_timestamp(time_t timestamp) {
    struct tm* timeinfo;
    char buffer[20];

    timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", timeinfo);

    return buffer;
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    free_leaderboard_entries(head_leaderboard_entry);
    head_leaderboard_entry = nullptr;
}
