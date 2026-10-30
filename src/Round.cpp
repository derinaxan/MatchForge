#include <iostream>
#include <stdexcept>
#include "Round.h"

Round getNextRound(Round round)
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

std::string printRound(Round round)
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
        default:
            break;
    }
    return temp;
}