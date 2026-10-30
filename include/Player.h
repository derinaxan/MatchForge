#pragma once 


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <vector>

#include "SportsType.h"
#include "Ranking.h" 

class Player 
{
private:
    boost::uuids::uuid id;
    std::string name;
    std::string surname;
    int strength;
    std::vector<Ranking> rankings;
    


public:
    Player(std::string& name, std::string& surname, int strength);
    Player(const boost::uuids::uuid& id, const std::string& name,  const std::string& surname, const int strength);

    const boost::uuids::uuid getId() const;

    const std::string getName() const;
    void setName(std::string& name);

    const std::string getSurname() const;
    void setSurname(std::string& surname);

    int getStrength() const; 
    void setStrength(int newStrength);

    const std::vector<Ranking>& getRankings() const;
    Ranking& getRanking(SportsType type);
    void addPoints(SportsType type, int points);
    int getPoints(SportsType type) const;
   
    bool operator==(const Player& other) const;

    void printSimple() const;
    void print() const;
    void printDetailed() const;
    void printByType(SportsType type) const;

};