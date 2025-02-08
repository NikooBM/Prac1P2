#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>

using namespace std;

const int kPLAYERNAME = 50;
const int kTEAMNAME = 40;
const int kPLAYERS = 5;
const int kMAXTEAMS = 20;

struct Player {
    char name[kPLAYERNAME];
    unsigned int goals;
    bool best;
};

struct Team {
    unsigned int id;
    char name[kTEAMNAME];
    unsigned int wins;
    unsigned int losses;
    unsigned int draws;
    unsigned int points;
    Player players[kPLAYERS];
};

enum Error {
    ERR_EMPTY,
    ERR_MAX_TEAMS,
    ERR_NO_LEAGUE,
    ERR_NO_TEAMS,
    ERR_NOT_EXIST,
    ERR_NUM_TEAMS,
    ERR_OPTION
};

void error(Error e) {
    switch(e) {
        case ERR_EMPTY: cout << "ERROR: empty string" << endl;
            break;
        case ERR_MAX_TEAMS: cout << "ERROR: maximum number of teams reached" << endl;
            break;
        case ERR_NO_LEAGUE: cout << "ERROR: there are no standings" << endl;
            break;
        case ERR_NO_TEAMS: cout << "ERROR: there are no teams" << endl;
            break;
        case ERR_NOT_EXIST: cout << "ERROR: team does not exist" << endl;
            break;
        case ERR_NUM_TEAMS: cout << "ERROR: wrong number of teams" << endl;
            break;
        case ERR_OPTION: cout << "ERROR: wrong option" << endl;
    }
}

void showMenu() {
    cout << "1- Add team" << endl
         << "2- Add all teams" << endl
         << "3- Delete team" << endl
         << "4- Show teams" << endl
         << "5- Play league" << endl
         << "6- Show standings" << endl
         << "7- Show best players" << endl
         << "q- Quit" << endl
         << "Option: ";
}

int simulateGoals(Team &team) {
    int goals = 0;
    for(int i = 0; i < kPLAYERS; i++) {
        int goal = rand() % 2;
        team.players[i].goals += goal;
        goals += goal;
    }
    return goals;
}

void addTeam(vector<Team> &teams, bool &leaguePlayed) {
    if (teams.size() >= kMAXTEAMS) {
        error(ERR_MAX_TEAMS);
        return;
    }

    Team team;
    team.id = teams.empty() ? 0 : teams.back().id + 1;
    
    cout << "Enter team name: ";
    cin.getline(team.name, kTEAMNAME);

    if (strlen(team.name) == 0) {
        snprintf(team.name, kTEAMNAME, "Team_%u", team.id);
    }

    team.wins = team.losses = team.draws = team.points = 0;

    for (int i = 0; i < kPLAYERS; i++) {
        snprintf(team.players[i].name, kPLAYERNAME, "%s-R%d", team.name, i + 1);
        team.players[i].goals = 0;
        team.players[i].best = false;
    }

    teams.push_back(team);
    leaguePlayed = false;
}

void addAllTeams(vector<Team> &teams, bool &leaguePlayed) {
    if (!teams.empty()) {
        char response;
        do {
            cout << "Do you want to delete existing teams (y/n)? ";
            cin >> response;
            cin.ignore();
            response = tolower(response);
        } while (response != 'y' && response != 'n');

        if (response == 'n') {
            return;
        }
        teams.clear();
    }

    unsigned int n;
    do {
        cout << "Enter number of teams: ";
        cin >> n;
        cin.ignore();
        if (n < 2 || n > kMAXTEAMS) {
            error(ERR_NUM_TEAMS);
        }
    } while (n < 2 || n > kMAXTEAMS);

    for (unsigned int i = 0; i < n; i++) {
        Team team;
        team.id = i;
        snprintf(team.name, kTEAMNAME, "Team_%u", i);
        team.wins = team.losses = team.draws = team.points = 0;

        for (int j = 0; j < kPLAYERS; j++) {
            snprintf(team.players[j].name, kPLAYERNAME, "%s-R%d", team.name, j + 1);
            team.players[j].goals = 0;
            team.players[j].best = false;
        }
        teams.push_back(team);
    }
    leaguePlayed = false;
}

void deleteTeam(vector<Team> &teams, bool &leaguePlayed) {
    if (teams.empty()) {
        error(ERR_NO_TEAMS);
        return;
    }

    cout << "Enter team name: ";
    char name[kTEAMNAME];
    cin.getline(name, kTEAMNAME);

    if (strlen(name) == 0) {
        error(ERR_EMPTY);
        return;
    }

    for (size_t i = 0; i < teams.size(); i++) {
        if (strcmp(teams[i].name, name) == 0) {
            teams.erase(teams.begin() + i);
            leaguePlayed = false;
            return;
        }
    }
    
    error(ERR_NOT_EXIST);
}

void showTeams(const vector<Team> &teams) {
    if (teams.empty()) {
        error(ERR_NO_TEAMS);
        return;
    }

    cout << "Enter team name: ";
    char name[kTEAMNAME];
    cin.getline(name, kTEAMNAME);

    if (strlen(name) == 0) {
        for (size_t i = 0; i < teams.size(); i++) {
            cout << "Name: " << teams[i].name << endl;
            cout << "Wins: " << teams[i].wins << endl;
            cout << "Losses: " << teams[i].losses << endl;
            cout << "Draws: " << teams[i].draws << endl;
            cout << "Points: " << teams[i].points << endl;
            for (int j = 0; j < kPLAYERS; j++) {
                cout << teams[i].players[j].name << ": " << teams[i].players[j].goals << " goals" << endl;
            }
            if (i < teams.size() - 1) cout << endl;
        }
        return;
    }

    for (const Team &team : teams) {
        if (strcmp(team.name, name) == 0) {
            cout << "Name: " << team.name << endl;
            cout << "Wins: " << team.wins << endl;
            cout << "Losses: " << team.losses << endl;
            cout << "Draws: " << team.draws << endl;
            cout << "Points: " << team.points << endl;
            for (int j = 0; j < kPLAYERS; j++) {
                cout << team.players[j].name << ": " << team.players[j].goals << " goals" << endl;
            }
            return;
        }
    }
    
    error(ERR_NOT_EXIST);
}

void playLeague(vector<Team> &teams, bool &leaguePlayed) {
    if (teams.size() < 2) {
        error(ERR_NUM_TEAMS);
        return;
    }

    // Reset all stats
    for (Team &team : teams) {
        team.wins = team.losses = team.draws = team.points = 0;
        for (int j = 0; j < kPLAYERS; j++) {
            team.players[j].goals = 0;
            team.players[j].best = false;
        }
    }

    // Play all matches
    for (size_t i = 0; i < teams.size() - 1; i++) {
        for (size_t j = i + 1; j < teams.size(); j++) {
            int goalsA = simulateGoals(teams[i]);
            int goalsB = simulateGoals(teams[j]);

            if (goalsA > goalsB) {
                teams[i].wins++;
                teams[i].points += 3;
                teams[j].losses++;
            } else if (goalsB > goalsA) {
                teams[j].wins++;
                teams[j].points += 3;
                teams[i].losses++;
            } else {
                teams[i].draws++;
                teams[j].draws++;
                teams[i].points++;
                teams[j].points++;
            }
        }
    }

    // Find best players
    for (Team &team : teams) {
        int maxGoals = -1;
        int bestPlayer = -1;
        for (int j = 0; j < kPLAYERS; j++) {
            if ((int)team.players[j].goals >= maxGoals) {
                maxGoals = team.players[j].goals;
                bestPlayer = j;
            }
        }
        if (bestPlayer >= 0) {
            team.players[bestPlayer].best = true;
        }
    }

    leaguePlayed = true;
}

void showStandings(const vector<Team> &teams, bool leaguePlayed) {
    if (!leaguePlayed) {
        error(ERR_NO_LEAGUE);
        return;
    }

    vector<Team> sortedTeams = teams;

    // Sort by points (bubble sort to maintain original order for equal points)
    for (size_t i = 0; i < sortedTeams.size() - 1; i++) {
        for (size_t j = 0; j < sortedTeams.size() - i - 1; j++) {
            if (sortedTeams[j].points < sortedTeams[j + 1].points) {
                Team temp = sortedTeams[j];
                sortedTeams[j] = sortedTeams[j + 1];
                sortedTeams[j + 1] = temp;
            }
        }
    }

    for (const Team &team : sortedTeams) {
        cout << team.name << "|"
             << team.wins << "|"
             << team.draws << "|"
             << team.losses << "|"
             << team.points << endl;
    }
}

void showBestPlayers(const vector<Team> &teams, bool leaguePlayed) {
    if (!leaguePlayed) {
        error(ERR_NO_LEAGUE);
        return;
    }

    for (const Team &team : teams) {
        for (int j = 0; j < kPLAYERS; j++) {
            if (team.players[j].best) {
                cout << team.name << "|"
                     << team.players[j].name << "|"
                     << team.players[j].goals << endl;
                break;
            }
        }
    }
}

int main() {
    char option;
    srand(888);

    vector<Team> teams;
    bool leaguePlayed = false;

    do {
        showMenu();
        cin >> option;
        cin.ignore();
        option = tolower(option);

        switch (option) {
            case '1':
                addTeam(teams, leaguePlayed);
                break;
            case '2':
                addAllTeams(teams, leaguePlayed);
                break;
            case '3':
                deleteTeam(teams, leaguePlayed);
                break;
            case '4':
                showTeams(teams);
                break;
            case '5':
                playLeague(teams, leaguePlayed);
                break;
            case '6':
                showStandings(teams, leaguePlayed);
                break;
            case '7':
                showBestPlayers(teams, leaguePlayed);
                break;
            case 'q':
                break;
            default:
                error(ERR_OPTION);
        }
    } while (option != 'q');

    return 0;
}