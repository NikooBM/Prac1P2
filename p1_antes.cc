// DNI 78008606N BIBILEISHVILI MAMALADZE, NIKOLOZ
#include <iostream> // cin, cout
#include <cstring> // strlen, strcmp
#include <cstdio> // sprintf

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

Team teams[kMAXTEAMS];
unsigned int numTeams = 0;
bool leaguePlayed = false;

enum Error {
    ERR_OPTION,
    ERR_MAX_TEAMS,
    ERR_NUM_TEAMS,
    ERR_NO_TEAMS,
    ERR_EMPTY,
    ERR_NOT_EXIST,
    ERR_NO_LEAGUE
};

void error(Error e) {
    switch (e) {
        case ERR_OPTION:
            cout << "ERROR: wrong menu option" << endl;
            break;
        case ERR_MAX_TEAMS:
            cout << "ERROR: maximum number of teams reached" << endl;
            break;
        case ERR_NUM_TEAMS:
            cout << "ERROR: invalid number of teams" << endl;
            break;
        case ERR_NO_TEAMS:
            cout << "ERROR: no teams available" << endl;
            break;
        case ERR_EMPTY:
            cout << "ERROR: empty name" << endl;
            break;
        case ERR_NOT_EXIST:
            cout << "ERROR: team does not exist" << endl;
            break;
        case ERR_NO_LEAGUE:
            cout << "ERROR: no league played" << endl;
            break;
    }
}

void showMenu() {
    cout << endl
         << "1- Add team" << endl
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

void addTeam() {
    if (numTeams >= kMAXTEAMS) {
        error(ERR_MAX_TEAMS);
        return;
    }
    // Referencia a un team para que se pueda modificar más fácilmente
    Team &team = teams[numTeams];
    team.id = numTeams;

    cout << "Enter team name: ";
    cin.getline(team.name, kTEAMNAME);

    if (strlen(team.name) == 0) {
        // Si name esta vacio, se guarda el nombre + el id
        //sprintf(team.name, "Team_%u", team.id);
        snprintf(team.name, kTEAMNAME, "Team_%u", team.id);
    }

    team.wins = team.losses = team.draws = team.points = 0;

    for (int i = 0; i < kPLAYERS; i++) {
        // Nombre del equipo + R + id del jugador
        //sprintf(team.players[i].name, "%s-R%d", team.name, i + 1);
        snprintf(team.players[i].name, kPLAYERNAME, "%s-R%d", team.name, i + 1);
        team.players[i].goals = 0;
        team.players[i].best = false;
    }

    numTeams++;
    leaguePlayed = false;
}

void addAllTeams() {
    if (numTeams > 0) {
        char response;
        do {
            cout << "Do you want to delete existing teams (y/n)? ";
            cin >> response;
            cin.ignore();
            response = tolower(response);
        } while (response != 'y' && response != 'n');

        if (response == 'n') return;
        numTeams = 0;
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
        Team &team = teams[i];
        team.id = i;
        //sprintf(team.name, "Team_%u", i);
        snprintf(team.name, kTEAMNAME, "Team_%u", i);
        team.wins = team.losses = team.draws = team.points = 0;

        for (int j = 0; j < kPLAYERS; j++) {
            //sprintf(team.players[j].name, "%s-R%d", team.name, j + 1);
            snprintf(team.players[j].name, kPLAYERNAME, "%s-R%d", team.name, j + 1);
            team.players[j].goals = 0;
            team.players[j].best = false;
        }
    }
    numTeams = n;
    leaguePlayed = false;
}

void deleteTeam() {
    if (numTeams == 0) {
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

    bool found = false;
    for (unsigned int i = 0; i < numTeams; i++) {
        if (strcmp(teams[i].name, name) == 0) {
            found = true;
            for (unsigned int j = i; j < numTeams - 1; j++) {
                teams[j] = teams[j + 1];
            }
            numTeams--;
            leaguePlayed = false;
            break;
        }
    }
    
    if (!found) {
        error(ERR_NOT_EXIST);
    }
}

void showTeams() {
    if (numTeams == 0) {
        error(ERR_NO_TEAMS);
        return;
    }

    cout << "Enter team name: ";
    char name[kTEAMNAME];
    cin.getline(name, kTEAMNAME);

    if (strlen(name) == 0) {
        for (unsigned int i = 0; i < numTeams; i++) {
            cout << "Name: " << teams[i].name << endl;
            cout << "Wins: " << teams[i].wins << endl;
            cout << "Losses: " << teams[i].losses << endl;
            cout << "Draws: " << teams[i].draws << endl;
            cout << "Points: " << teams[i].points << endl;
            for (int j = 0; j < kPLAYERS; j++) {
                cout << teams[i].players[j].name << ": " << teams[i].players[j].goals << " goals" << endl;
            }
            if (i < numTeams - 1) cout << endl;
        }
    } else {
        bool found = false;
        for (unsigned int i = 0; i < numTeams && !found; i++) {
            if (strcmp(teams[i].name, name) == 0) {
                found = true;
                cout << "Name: " << teams[i].name << endl;
                cout << "Wins: " << teams[i].wins << endl;
                cout << "Losses: " << teams[i].losses << endl;
                cout << "Draws: " << teams[i].draws << endl;
                cout << "Points: " << teams[i].points << endl;
                for (int j = 0; j < kPLAYERS; j++) {
                    cout << teams[i].players[j].name << ": " << teams[i].players[j].goals << " goals" << endl;
                }
            }
        }
        if (!found) {
            error(ERR_NOT_EXIST);
        }
    }
}

void playLeague() {
    if (numTeams < 2) {
        error(ERR_NUM_TEAMS);
        return;
    }

    // Reset all stats
    for (unsigned int i = 0; i < numTeams; i++) {
        teams[i].wins = teams[i].losses = teams[i].draws = teams[i].points = 0;
        for (int j = 0; j < kPLAYERS; j++) {
            teams[i].players[j].goals = 0;
            teams[i].players[j].best = false;
        }
    }

    // Play all matches
    for (unsigned int i = 0; i < numTeams - 1; i++) {
        for (unsigned int j = i + 1; j < numTeams; j++) {
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
    for (unsigned int i = 0; i < numTeams; i++) {
        int maxGoals = -1;
        int bestPlayer = 0;
        for (int j = 0; j < kPLAYERS; j++) {
            if (teams[i].players[j].goals > maxGoals) {
                maxGoals = teams[i].players[j].goals;
                bestPlayer = j;
            }
        }
        teams[i].players[bestPlayer].best = true;
    }

    leaguePlayed = true;
}

void showStandings() {
    if (!leaguePlayed) {
        error(ERR_NO_LEAGUE);
        return;
    }

    // Create a copy of teams array for sorting
    Team sortedTeams[kMAXTEAMS];
    for (unsigned int i = 0; i < numTeams; i++) {
        sortedTeams[i] = teams[i];
    }

    // Sort teams by points (bubble sort)
    for (unsigned int i = 0; i < numTeams - 1; i++) {
        for (unsigned int j = 0; j < numTeams - i - 1; j++) {
            if (sortedTeams[j].points < sortedTeams[j + 1].points) {
                Team temp = sortedTeams[j];
                sortedTeams[j] = sortedTeams[j + 1];
                sortedTeams[j + 1] = temp;
            }
        }
    }

    // Show standings
    for (unsigned int i = 0; i < numTeams; i++) {
        cout << sortedTeams[i].name << "|"
             << sortedTeams[i].wins << "|"
             << sortedTeams[i].draws << "|"
             << sortedTeams[i].losses << "|"
             << sortedTeams[i].points << endl;
    }
}

void showBestPlayers() {
    if (!leaguePlayed) {
        error(ERR_NO_LEAGUE);
        return;
    }

    for (unsigned int i = 0; i < numTeams; i++) {
        for (int j = 0; j < kPLAYERS; j++) {
            if (teams[i].players[j].best) {
                cout << teams[i].name << "|"
                     << teams[i].players[j].name << "|"
                     << teams[i].players[j].goals << endl;
                break;
            }
        }
    }
}

int main() {
    char option;
    srand(888);

    do {
        showMenu();
        cin >> option;
        cin.ignore();

        switch (option) {
            case '1':
                addTeam();
                break;
            case '2':
                addAllTeams();
                break;
            case '3':
                deleteTeam();
                break;
            case '4':
                showTeams();
                break;
            case '5':
                playLeague();
                break;
            case '6':
                showStandings();
                break;
            case '7':
                showBestPlayers();
                break;
            case 'q':
                break;
            default:
                error(ERR_OPTION);
        }
    } while (option != 'q');

    return 0;
}