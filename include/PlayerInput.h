#pragma once 

#include <string> 

struct PlayerInput
{
    //referacnce yaamiyomusuz eger referance veririsek construcor gerekiyormus cunlu stringin neye referacne olacaignin bilinneis gerekiyormus referance null gosermiyor diye

    std::string name;
    std::string surname;
    int strength;
};