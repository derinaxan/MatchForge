#pragma once 

#include <string> 

#include "SportsType.h"
#include "TournamentSize.h"

struct TournamentInput
{
    std::string name ;
    int year;
    SportsType type;
    TournamentSize size;
};