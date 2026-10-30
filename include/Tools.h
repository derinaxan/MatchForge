#pragma once 

#include "TournamentSize.h"
#include "Round.h"
#include "Score.h"
#include "Ranking.h"

class Tools
{
private:

public:
    static Round convertTournamentSizeToRound(const TournamentSize& size);
    static Round getNextRound(const Round& round);
    static void printRound(const Round& round);
    static void printScore(const Score& score);
    static void printRanking(const Ranking& ranking);
    static int getWinPoints(const SportsType& type);
    static int getLosePoints(const SportsType& type);
    static std::string sportsTypeEnumToString(const SportsType& type);
    static SportsType sportsTypeStringToEnum(const std::string& type);
    static Round roundStringToEnum(const std::string& type);
    static std::string roundEnumToString(const Round round);
    static TournamentSize sizeIntToEnum(const int size);
    static int sizeEnumToInt(const TournamentSize size);
    
};
