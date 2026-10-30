#include <vector> 
#include <iostream>
#include <algorithm>

#include "Tournament.h"
#include "Round.h"
#include "Tools.h"

Tournament::Tournament(std::string& name, int year, SportsType type, TournamentSize size)
    :id(boost::uuids::random_generator()()), name(name), year(year), type(type), size(size)
    {
        currentRound = Tools::convertTournamentSizeToRound(size);
    }
Tournament::Tournament(boost::uuids::uuid id, const std::string& name, int year, SportsType type, TournamentSize size, Round currentRound)
    :id(id), name(name), year(year), type(type), size(size), currentRound(currentRound)
    {}


boost::uuids::uuid Tournament::getId() const
{
    return id;
}

void Tournament::setId(boost::uuids::uuid id)
{
    id = id;
}

const std::string&  Tournament::getName() const
{
    return name;
};

void Tournament::setName(std::string& name)
{
    name = name;
};

bool Tournament::getHasStarted() const
{
    return hasStarted;
}

void Tournament::setHasStarted(bool b){
    hasStarted = b;
}

int Tournament::getYear() const
{   
    return year; 
};

void Tournament::setYear(int year)
{
    year = year;
}

Player* Tournament::getWinner() const
{
    return winner;
}
void Tournament::setWinner(Player& player)
{
    winner = &player;
}

std::vector<Match>& Tournament::getMatches()
{
    return matches;

}

const std::vector<Match>& Tournament::getMatches() const
{
    return matches;
}

void Tournament::addMatch(const Match& match)
{
    matches.push_back(match);
}

const std::vector<Player*> Tournament::getPlayers() const
{
    return players;
}

bool Tournament::addPlayer(Player& player)
{
     if(containsPlayer(player))
    {
       return false;
    }
    players.push_back(&player);
    return true;
}

bool Tournament::removePlayer(const Player& player)
{
      //erase kullanamiyoruz cunku erase direk iterator aliyor parametre olarak
    //erase if kac elemean silfigini return ediyor, parametre olarak vektoru ve conditionu aliyor
    auto removed = std::erase_if( players, 
        [&player](Player* p)
        {
            return *p == player;
        }
    );
    if(removed == 0) return false;
    return true;
}

bool  Tournament::containsPlayer(const Player& player)
{
    for(Player* p:players)
    {
        if(p->getId() == player.getId())
        {
            return true;
        }
    }
    return false;
}

SportsType Tournament::getType() const
{
    return type;
}

void Tournament::setType(SportsType type)
{
    type = type;
}

Round Tournament::getCurrentRound() const
{
    return currentRound;
}

void Tournament::setCurrentRound(Round round)
{
    currentRound = round;
}

TournamentSize Tournament::getSize() const
{
    return size;
}

void Tournament::setSize(TournamentSize size)
{
    size = size; 
}

void Tournament::print() const
{
    std::cout << name << " (" << year << ")"  << " | " << Tools::sizeEnumToInt(size) <<  " | " << Tools::sportsTypeEnumToString(type)  << std::endl;
}

void Tournament::printDetailed() const
{
    std::cout << "tournament name: " << name << "\n";
    std::cout << "matches: " << std::endl;
    for(const Match& m: matches)
    {
        m.print();

    }
    std::cout << "\nplayers:" << std::endl;
    for(Player* p: players)
    {
        p->print();   
    }

    std::cout << "current tournament round: "; 
    Tools::printRound(currentRound);
    std::cout << "\ntournmanet size: " << static_cast<int>(size);
    if(winner != nullptr)
    {
        std::cout << "\nwinner "; 
        winner->printSimple();
    }
    else
    {
        std::cout << "\nwinner has not been determined yet" << std::endl;
    }
    
   
};

