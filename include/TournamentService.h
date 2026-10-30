#pragma once 


#include <optional> 

#include "Tournament.h"
#include "PlayerService.h"

class TournamentService
{
private:
    std::vector<Tournament>& tournaments;
    std::vector<Match*> getUpComingMatches(Tournament& tournament);
    std::vector<Match> calculateNewMatchups(std::vector<Player*> winners, Round round);

public:
    TournamentService(std::vector<Tournament>& tournaments);
    void start(Tournament& t);
    void simulateRound(Tournament& tournament);
    void createTournament(std::string& name, int year, SportsType type, TournamentSize size);
    std::vector<Tournament*> getTournamentByName(std::string& name);
   
};