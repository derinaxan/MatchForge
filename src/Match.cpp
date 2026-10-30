#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Score.h"
#include "Match.h"
#include "Round.h"
#include "Tools.h"



Match::Match(Player& playerOne, Player& playerTwo, Round round)
    :id(boost::uuids::random_generator()()), homePlayer(playerOne), awayPlayer(playerTwo), score{0, 0}, round(round)
    {}

Match::Match(boost::uuids::uuid id, Player& playerOne, Player& playerTwo, Round round)
    :id(id), homePlayer(playerOne), awayPlayer(playerTwo), score{0, 0}, round(round)
    {}
    
const boost::uuids::uuid Match::getId() const
{
    return id;
}
 
Player& Match::getHomePlayer() const
{
    return homePlayer;
}

void Match::setHomePlayer(Player& player)
{
    homePlayer = player;
}

Player& Match::getAwayPlayer() const
{   
    return awayPlayer;
}

void Match::setAwayPlayer(Player& player)
{
    awayPlayer = player;
}

void Match::setScore(Score score) 
{
    this->score = score;
}
Score Match::getScore() const
{
    return score;
}

void Match::updateScore(int performance1, int performance2, SportsType type)
{
    switch (type)
    {
        case SportsType::FOOTBALL:
            score.homeScore = performance1 / 2000;
            score.awayScore = performance2 / 2000;
            break;

        case SportsType::BASKETBALL:
            score.homeScore = 60 + performance1 / 100;
            score.awayScore = 60 + performance2 / 100;
            break;
    }
    if (score.homeScore == score.awayScore)
    {
        if (performance1 > performance2)
            score.homeScore++;
        else
            score.awayScore++;
    }
}



const Round Match::getRound() const
{
    return round;
}

void Match::setRound(Round round)
{
    round = round;
}


Player& Match::simulate(SportsType type)
{
    int homePerformance =  homePlayer.getStrength() * (rand() % 100 + 1);
    int awayPerformance = awayPlayer.getStrength() * (rand() % 100 + 1);

    updateScore(homePerformance, awayPerformance, type);

    if (homePerformance > awayPerformance)
    {
        homePlayer.addPoints(type, Tools::getWinPoints(type));
        awayPlayer.addPoints(type, Tools::getLosePoints(type));
        return homePlayer;
    }
    homePlayer.addPoints(type, Tools::getLosePoints(type));
    awayPlayer.addPoints(type, Tools::getWinPoints(type));
    return awayPlayer;
}


void Match::print() const
{ 
    std::cout << "home ";
    homePlayer.print();
    std::cout << "away player ";
    awayPlayer.print();
    Tools::printScore(score);
    std::cout << "Match round: ";
    Tools::printRound(round);

};