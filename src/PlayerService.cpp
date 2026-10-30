#include <iostream> 
#include <queue>
#include <vector>
#include <algorithm> 

#include "PlayerService.h"
#include "Player.h"

PlayerService::PlayerService(std::vector<Player>& players)
    :players(players)
    {}

Player& PlayerService::createPLayer(std::string& name, std::string& surname, int strength)
{
    Player p(name, surname, strength);
    players.push_back(p);
    return players.back();
}
//complexity

std::vector<Player*> PlayerService::getMathcingPlayersByNameSurname(std::string& name, std::string& surname) const
{
    std::vector<Player*> matchingPlayers;
    for(Player& p: players)
    {
        if(p.getName() == name && p.getSurname() == surname)
        {
            matchingPlayers.push_back(&p);
        }
    }
    return matchingPlayers;
}

void PlayerService::displayLeaderboard(SportsType type) const
{
    if(players.empty())
    {
        std::cout << "no players yet" << std::endl;
        return;
    }

    std::vector<const Player*> leaderboard;

    for(const Player& p : players)
    {
        leaderboard.push_back(&p);
    }

    std::sort(
        leaderboard.begin(),
        leaderboard.end(),
        [type](const Player* a, const Player* b)
        {
            return a->getPoints(type) > b->getPoints(type);
        }
    );

    for(int i = 0; i < leaderboard.size() && i < 5; i++)
    {
        std::cout << "\n" << i + 1 << "-";
        leaderboard[i]->printByType(type);
    }
}
