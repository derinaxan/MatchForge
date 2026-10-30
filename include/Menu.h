
#pragma once 

#include "SportsType.h"
#include "PlayerInput.h"
#include "TournamentInput.h"

class Menu
{
private:
    

public:
    static int mainMenu() ;
    static PlayerInput getCreatePlayerInput();  
    static PlayerInput getSearchPlayerInput();
    static TournamentInput getCraeteTournamentInput();
    static TournamentInput getSearchTournamentInput();
    static SportsType getSportsTypeInput();
    static std::string getNameInput();
    static TournamentSize getTournamentSizeInput();
    static int getNumberInput(int min, int max);
    static void terminate();
    static void successfull();
    static void enter();
    static void unsuccessfull();  
    static int playerOptions();
    static int tournamentOptions();
    
};