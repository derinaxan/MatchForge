#pragma once 

#include <string>
#include <vector>
#include <optional>



#include <boost/uuid/uuid.hpp>


#include "Player.h"
#include "Match.h"
#include "SportsType.h"
#include "Round.h"
#include "TournamentSize.h"


class Tournament 
{
private:
    boost::uuids::uuid id;
    std::string name;
    int year; 
    bool hasStarted = false;

    std::vector<Match> matches;
    
    std::vector<Player*> players;
    SportsType type;
    Round currentRound;
    TournamentSize size;
    Player* winner = nullptr;



public:
    Tournament(std::string& name, int year, SportsType type, TournamentSize size);
    Tournament(boost::uuids::uuid id, const std::string& name, int year, SportsType type, TournamentSize size, Round currentRound);

    boost::uuids::uuid  getId() const;
    void setId(boost::uuids::uuid id);

    const std::string& getName() const;
    void setName(std::string& name);

    bool getHasStarted() const;
    void setHasStarted(bool b);

    int getYear() const;
    void setYear(int year);

    Player* getWinner() const;
    void setWinner(Player& player);

    std::vector<Match>& getMatches();
    const std::vector<Match>& getMatches() const;
    void addMatch(const Match& match);

    const std::vector<Player*> getPlayers() const;
    bool addPlayer(Player& player);
    bool removePlayer(const Player& player);
    bool containsPlayer(const Player& player);

    SportsType getType() const;
    void setType(SportsType type); 

    Round getCurrentRound() const;
    void setCurrentRound(Round round);

    TournamentSize getSize() const;
    void setSize(TournamentSize size);

    void print() const;
    void printDetailed() const;

};