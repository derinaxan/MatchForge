#pragma once 

#include <string>
#include <vector>

#include "Tournament.h"
#include "TournamentService.h"
#include "PlayerService.h"
#include "Menu.h"
#include "DBManager.h"

class MainManager
{
private:
    std::vector<Tournament> tournaments;
    std::vector<Player> players;

    TournamentService tournamentService;
    PlayerService playerService;
    DBManager dbManager;

    bool initilzed = false;
    
public: 
    MainManager();
    ~MainManager();

    MainManager(const MainManager&) = delete;  //yeni mainmanager olusututrlmasini yasakliyor delete yasaklamak demek
    MainManager& operator=(const MainManager&) = delete;  //mevcut MainManager'in digerine atilmasini yasakliyor 

    void initilize();
    void save();

    void createPlayer(std::string& name, std::string& surname, int strength);
    std::vector<Player*> searchPlayer(std::string& name, std::string& surname);
    void createTournament(std::string& name, int year, SportsType  type, TournamentSize size);
    std::vector<Tournament*> searchTournament(std::string& name);

    bool addPlayerToTournament(Player* player, Tournament* tournament);
    bool removePlayerFromTournament(Player* player, Tournament* tournament);

    std::vector<Tournament*> getPlayedTournaments(Player& player);
    std::vector<Tournament*> getNonPlayedTournaments(Player& player);

    void startTournament(Tournament& tournament);
    void simulateRound(Tournament& tournament);

    std::vector<Player> getLeaderBoard(SportsType type) const; 
    
};