#include <iostream>
#include <limits>

#include "Menu.h"
#include "PlayerInput.h"
#include "TournamentInput.h"
#include "SportsType.h"
#include "TournamentSize.h"


int  Menu::mainMenu() 
{
    std::cout <<  "Main menu\n" << 
    "1-Create player\n" << 
    "2-Create tournament\n" <<
    "3-Search Player\n" <<  // ismi soyIsmi olan butun o playerlari getirsin onun icinen user secsin
    "4-Search tournament\n" <<  //burad simulateRound icerigi var sadece deleteTOURNAMENT diyince matchlerin revertlenmesi gerekiyor cunku
    "5-View Leaderboard\n" << 
    "6-Exit" << std::endl;
    return getNumberInput(1, 6);
}
 
int Menu::getNumberInput(int min, int max) 
{
    int input;
    while(true)
    {
        std::cin >> input;
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if(input < min || input > max)
        {
            std::cout << "enter a number between " <<  min  << " and "  << max << std::endl;
            continue;
        }
        return input;
    }

}

PlayerInput Menu::getSearchPlayerInput()
{
    PlayerInput p;
    std::cout << "enter player name" << std::endl;
    std::cin >> p.name;
    std::cout << "enter player surname" << std::endl;
    std::cin >> p.surname;
    return p;
    
}

PlayerInput Menu::getCreatePlayerInput()
{   
    PlayerInput player;
    std::cout << "enter player name" << std::endl;
    std::cin >> player.name;
    std::cout << "enter player surname" << std::endl;
    std::cin >> player.surname;
    std::cout << "enter player strength" << std::endl;
    std::cin >> player.strength;
    return player;
};

TournamentInput Menu::getSearchTournamentInput()
{
    TournamentInput tournament;
    std::cout << "enter tournament name" << std::endl;
    std::cout << "0-Back" << std::endl;
    std::cin >> tournament.name;
    return tournament;
};

TournamentInput Menu::getCraeteTournamentInput()
{
    TournamentInput tournament;
    std::cout << "enter tournament name" << std::endl;
    std::cin >> tournament.name;
    std::cout << "enter tournament year" << std::endl;
    std::cin >> tournament.year;
    tournament.type = getSportsTypeInput();
    tournament.size = getTournamentSizeInput();
    return tournament;
};



TournamentSize Menu::getTournamentSizeInput()
{

    std::cout << "pick a size\n"
    << "1-32\n"
    << "2-16\n" 
    << "3-8\n"
    << "4-4\n"
    << "5-2" << std::endl;
    int input = getNumberInput(0, 6);
    switch(input)
    {
        case 1: return TournamentSize::SIZE_32; 
        case 2: return TournamentSize::SIZE_16;
        case 3: return TournamentSize::SIZE_8;
        case 4: return TournamentSize::SIZE_4;
        case 5: return TournamentSize::SIZE_2;
        default:
            throw std::invalid_argument("invalid tournament size");
    }

};

SportsType Menu::getSportsTypeInput()
{
    std::cout << "pick a sports type\n"   
    << "1-FOOTBALL\n" << "2-BASKETBALL\n";
    int input = getNumberInput(1, 2);
    switch(input)
    {
        case 1: return SportsType::FOOTBALL;
        case 2: return SportsType::BASKETBALL;
        default:
            throw std::invalid_argument("invalid argument lele");
    }

    
};

void Menu::terminate()
{
    std::cout << "terminated" << std::endl;
}

void Menu::successfull()
{
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Menu::enter()
{
    std::cout << "Press Enter to try again...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Menu::unsuccessfull()
{
    std::cout << "unsuccessfull" << std::endl;
}

int Menu::playerOptions()
{   
    //4 implemnet edilmedi 
    std::cout << "1-view player stats\n" << "2-add Player to tournament\n" << "3-remove player from tournament\n" << "0-back\n" << std::endl;
    return getNumberInput(0, 3); ////printDetailedStats 
}
int Menu::tournamentOptions()
{
    std::cout << "1-look at detailed stats\n" << "2-Start tournament\n" << "3-Simulate current Round\n" << "0-Back" << std::endl; 
    return getNumberInput(0, 3);
};

std::string getNameInput()
{   
    std::string temp;
    std::cout << "enter name" << std::endl;
    std::cin >> temp;
    return temp;
}