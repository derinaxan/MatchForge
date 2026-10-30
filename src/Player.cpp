#include <iostream>
#include <string> 

#include "Player.h"
#include "Tools.h"




Player::Player(std::string& name, std::string& surname, int strength)
    :name(name), surname(surname), strength(strength), id(boost::uuids::random_generator()()) 
    {}

Player::Player(const boost::uuids::uuid& id, const std::string& name,  const std::string& surname, const int strength)
    :id(id), name(name), surname(surname), strength(strength)
    {}
    
const boost::uuids::uuid  Player::getId()  const
{
    return id;
};

const std::string Player::getName() const
{
    return name;
}

void Player::setName(std::string& newName)
{
    name = newName;
} 

const std::string Player::getSurname() const
{
    return surname;
}
void Player::setSurname(std::string& surname)
{
    surname = surname; 
}

int Player::getStrength() const
{
    return strength;
}

void Player::setStrength(int newStrength)
{
    strength =  newStrength;
}

const std::vector<Ranking>& Player::getRankings() const
{
    return rankings;
}

Ranking& Player::getRanking(SportsType type) 
{
    for (Ranking& ranking : rankings)
    {
        if (ranking.type == type)
            return ranking;
    }

    throw std::runtime_error("Ranking not found");
}

void Player::addPoints(SportsType type, int points)
{
    for (Ranking& r : rankings)
    {
        if (r.type == type)
        {
            r.points += points;
            return;
        }
    }

    rankings.push_back({type, points});
}


int Player::getPoints(SportsType type) const
{
    for(const Ranking& ranking:rankings)
    {
        if(ranking.type == type) return ranking.points;
    }
    return 0;
}

bool Player::operator==(const Player& other) const
{
    return id == other.id;
}


void Player::printSimple() const 
{
       std::cout << "name surname: " <<  name << " " << surname << std::endl;
}
void Player::print() const
{
    std::cout << "player name : " << name << " | "  << 
    "surname: " << surname << " |  " << 
    "strength: " << strength << std::endl;
}

void Player::printDetailed() const
{
    std::cout << "player name : " << name << " | "  << 
    "surname: " << surname << " |  " << 
    "strength: " << strength << "\n" << 
    "rankings: " << std::endl;
    for(const Ranking& r :rankings)
    {
        Tools::printRanking(r);
        std::cout << " ";
    } 
};

void Player::printByType(SportsType type) const
{
    std::cout << "player name : " << name << " | "  << 
    "surname: " << surname << " |  " << 
    "strength: " << strength << " | ";
    for(const Ranking& r :rankings)
    {
        if(r.type == type)
        {
            Tools::printRanking(r);
            std::cout << " ";
        }
    } 
};