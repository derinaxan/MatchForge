#pragma once 


#include <boost/uuid/uuid.hpp>

#include "Player.h"

class PlayerService 
{
private:
    std::vector<Player>& players;
    
public:
    PlayerService(std::vector<Player>& players);
    Player& createPLayer(std::string& name, std::string& surname, int strength);
    std::vector<Player*> getMathcingPlayersByNameSurname(std::string& name, std::string& surname) const;
    void displayLeaderboard(SportsType type) const;
    
};