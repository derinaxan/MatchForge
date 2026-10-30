
#include <iostream> 

#include <boost/uuid/uuid.hpp>
#include <algorithm>
#include <random>


#include "TournamentService.h"
#include "Tournament.h"
#include "Tools.h"


std::vector<Match*> TournamentService::getUpComingMatches(Tournament& tournament)
{
    //referance falan doenemiyoruz cunku bu upcoingler fonskiyonun icinde olsuyurulmus local variable bunun referance olma ihtimali yok
    std::vector<Match>& matches = tournament.getMatches();
    std::vector<Match*> upcoming;
    for(Match& m: matches)
    {
        if(m.getRound() == tournament.getCurrentRound())
        {
            upcoming.push_back(&m);
        }
    }
    return upcoming;
};


std::vector<Match> TournamentService::calculateNewMatchups(std::vector<Player*> remainingPlayers, Round round)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(remainingPlayers.begin(), remainingPlayers.end(), gen);
    std::vector<Match> newRound;
    for(int i=0;i<remainingPlayers.size();i+=2)
    {
        std::cout << remainingPlayers.size() << "  " <<  i << " i+1 " << i << std::endl; 
        Match match(*remainingPlayers[i], *remainingPlayers[i+1], round);
        newRound.push_back(match);
    }
    return newRound; 
    
}


TournamentService::TournamentService(std::vector<Tournament>& tournaments)
    :tournaments(tournaments)
    {}
    

void TournamentService::createTournament(std::string& name, int year, SportsType type, TournamentSize size)
{
    Tournament t(name, year, type, size);
    tournaments.push_back(t);
}

void TournamentService::start(Tournament& tournament)
{
    //eger baslatmauya hazirsa basllat ve successful yaz else ise yazame 
    if(tournament.getHasStarted() == true)
    {
        std::cout << "tournament has already started" << std::endl;
        return;
    }
    if(tournament.getPlayers().size() < static_cast<int>(tournament.getSize()))
    {
        std::cout << "not able to start tournament" << std::endl;
        return;
    }
     tournament.setHasStarted(true);
     std::vector<Match> nextRound = calculateNewMatchups(tournament.getPlayers(),  tournament.getCurrentRound());
     for(Match& m:nextRound)
     {
        tournament.addMatch(m);
     }
     std::cout << "tournament started successfully" << std::endl;
}

void TournamentService::simulateRound(Tournament& tournament)
{
    if(tournament.getHasStarted() == false)
    {
        std::cout << "tournament has not started yet" << std::endl;
        return;
    }
    else if(tournament.getCurrentRound() == Round::FINISHED)
    {
        std::cout << "tournament has already finished" << std::endl;
        return;
    }
    std::vector<Match*> upcomingMatches = getUpComingMatches(tournament);
    std::vector<Player*> winners; 
    for(Match* m : upcomingMatches)
    {
        winners.push_back(&(m->simulate(tournament.getType())));
    }
    if(tournament.getCurrentRound() == Round::FINAL)
    {
        tournament.setWinner(*winners[0]);
        tournament.setCurrentRound(Round::FINISHED);
        return;
    }
    std::vector<Match> nextRound = calculateNewMatchups(winners,  Tools::getNextRound(tournament.getCurrentRound()));
    for(Match& m: nextRound)
    {
        tournament.addMatch(m);
    }
    Round next = Tools::getNextRound(tournament.getCurrentRound());
    tournament.setCurrentRound(next);
}



std::vector<Tournament*> TournamentService::getTournamentByName(std::string& name)
{   
    std::vector<Tournament*> matches;
    for(Tournament& t : tournaments)
    {   
        if(t.getName() == name)
        {
            matches.push_back(&t);
        }
    }
    return matches;
};


