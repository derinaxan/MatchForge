#include <stdexcept>

#include "Tools.h"

Round Tools::convertTournamentSizeToRound(const TournamentSize& size)
{
    switch(size)
    {
        case TournamentSize::SIZE_32: return Round::LAST_32;
        case TournamentSize::SIZE_16: return Round::LAST_16;
        case TournamentSize::SIZE_8: return Round::QUARTER_FINALS;
        case TournamentSize::SIZE_4: return Round::SEMI_FINALS;
        case TournamentSize::SIZE_2: return Round::FINAL;
        default:
            throw std::invalid_argument("invalid argument tournament size realted");
    }
    throw std::runtime_error("invalid");
}

Round Tools::getNextRound(const Round& round)
{
    switch(round)
    {
        case Round::LAST_32:
            return Round::LAST_16;
        case Round::LAST_16:
            return Round::QUARTER_FINALS;
        case Round::QUARTER_FINALS:
            return Round::SEMI_FINALS;
        case Round::SEMI_FINALS:
            return Round::FINAL;
        case Round::FINAL: 
            return Round::FINISHED;
        case Round::FINISHED:
            throw std::runtime_error("cannot advance the final");

    }
    throw std::runtime_error("invalid round");
}

void Tools::printRound(const Round& round)
{
    std::string temp;
    switch(round)
    {
        case Round::LAST_32:
            temp = "Last 32";
            break;
        case Round::LAST_16:
            temp = "Last 16";
            break;    
        case Round::QUARTER_FINALS:
            temp = "Last 8";
            break;  
        case Round::SEMI_FINALS:
            temp = "semi finals";
            break;  
        case Round::FINAL:
            temp = "final";
            break;  
        case Round::FINISHED:
            temp = "finished";
            break;
        default:
            break;
    }
    std::cout <<  temp;
}

void Tools::printScore(const Score& score) 
{
    std::cout << "home score: " << score.homeScore << std::endl;
    std::cout << "away score: " << score.awayScore << std::endl;
}

void Tools::printRanking(const Ranking& ranking)
{
    std::cout << "sport type: ";
    switch(ranking.type)
    {
        case SportsType::BASKETBALL:
            std::cout << "Basketball";
            break;
        case SportsType::FOOTBALL:
            std::cout << "Football";
            break;
        default:
            return;
    }
    std::cout <<  " | points: " << ranking.points;  
    
}

std::string Tools::sportsTypeEnumToString(const SportsType& type)
{
    switch (type)
    {
        case SportsType::FOOTBALL:
            return "FOOTBALL";

        case SportsType::BASKETBALL:
            return "BASKETBALL";
    }

    throw std::runtime_error("Invalid sports type");
}

TournamentSize Tools::sizeIntToEnum(const int size)
{
    if(size == 32) return TournamentSize::SIZE_32;
    else if(size == 16) return TournamentSize::SIZE_16;
    else if(size ==  8) return TournamentSize::SIZE_8;
    else if(size == 4)  return TournamentSize::SIZE_4;
    else if(size == 2) return TournamentSize::SIZE_2;
    throw std::invalid_argument("not an actual size");
}

int Tools::getWinPoints(const SportsType& type)
{
    switch (type)
    {
        case SportsType::FOOTBALL:
            return 3;

        case SportsType::BASKETBALL:
            return 2;
    }

    throw std::runtime_error("Invalid sports type");
}

int Tools::getLosePoints(const SportsType& type)
{
    switch (type)
    {
        case SportsType::FOOTBALL:
            return 0;

        case SportsType::BASKETBALL:
            return 1;
    }

    throw std::runtime_error("Invalid sports type");
}

SportsType Tools::sportsTypeStringToEnum(const std::string& type)
{
    if(type == "FOOTBALL") return SportsType::FOOTBALL;
    if(type == "BASKETBALL") return SportsType::BASKETBALL;
    throw std::invalid_argument("not an actual type");
}

 Round Tools::roundStringToEnum(const std::string& round)
{
    if(round == "LAST_32") return Round::LAST_32;
    if(round == "LAST_16") return Round::LAST_16;
    if(round == "QUARTER_FINALS" ) return Round::QUARTER_FINALS;
    if(round == "SEMI_FINALS") return  Round::SEMI_FINALS;
    if(round == "FINAL") return Round::FINAL;
    if(round == "FINISHED") return Round::FINISHED;
    throw  std::invalid_argument("not an actual round");
}


std::string Tools::roundEnumToString(const Round round)
{
    switch(round)
    {
        case(Round::LAST_32): return "LAST_32";
        case(Round::LAST_16): return "LAST_16";
        case(Round::QUARTER_FINALS): return "QUARTER_FINALS";
        case(Round::SEMI_FINALS): return "SEMI_FINALS";
        case(Round::FINAL): return "FINAL";
        case(Round::FINISHED): return "FINISHED";
        default:
            throw std::invalid_argument("invalid argument");
    }
}

int Tools::sizeEnumToInt(const TournamentSize size)
{
    switch(size)
    {
        case TournamentSize::SIZE_32: return 32;
        case TournamentSize::SIZE_16: return 16;
        case TournamentSize::SIZE_8: return 8;
        case TournamentSize::SIZE_4: return 4;
        case TournamentSize::SIZE_2: return 2;
        default:
            throw std::invalid_argument("invalid argument entered");
    }
    
}

