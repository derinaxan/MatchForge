#pragma once 

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <optional> 

#include "Round.h"
#include "Player.h"
#include "Score.h"



//simulate ve updateScore metthodlarini checkle bir daha 
class Match 
{
private:
    boost::uuids::uuid id;
    Player& homePlayer;
    Player& awayPlayer;
    Score score;
    Round round;

    
    

public: 
    Match(Player& playerOne, Player& playerTwo, Round round);
    Match(boost::uuids::uuid id, Player& playerOne, Player& playerTwo, Round round);

    const boost::uuids::uuid getId() const; 

    Player& getHomePlayer() const;
    void setHomePlayer(Player& player);

    Player& getAwayPlayer() const;
    void setAwayPlayer(Player& player);

    void setScore(Score score); 
    Score getScore() const;
    void updateScore(int homeScore, int awayScore, SportsType type);

    const Round getRound() const;
    void setRound(Round round);


    Player& simulate(SportsType type);  
    void print() const; 



};