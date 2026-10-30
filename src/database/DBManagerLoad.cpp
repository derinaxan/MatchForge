#include "DBManager.h"

bool DBManager::loadPlayers(std::vector<Player>& players)
{
    if(!players.empty())
    {
        std::cout << "players must be empty before loading" << std::endl;
        return false;
    }
    const char* sqlCommand = "SELECT id, name, surname, strength FROM players;";
    sqlite3_stmt* statement  = nullptr;
    int returnCode = sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr); //yazilan sql commandinin sqlitin anlaaybilecegi bir formata ceveiriyor 
    if(returnCode != SQLITE_OK)
    {
       std::cout << "failed to read load player statement" << std::endl;
       return false;
    }

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        const boost::uuids::uuid id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        const std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
        const std::string surname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
        const int strength = sqlite3_column_int(statement, 3);
        players.emplace_back(id , name, surname, strength);
    }
    sqlite3_finalize(statement); // Artık sorguyla işin bitti sorguyla is bitince kullandigi kaynaklari serbest birakir 
    return true;
}



bool DBManager::loadTournaments(std::vector<Tournament>& tournaments,  std::vector<Player>& players)
{

    const char* sqlCommand = "SELECT id, name, year, sport_type, size, has_started, current_round, winner_id FROM tournaments;";
    sqlite3_stmt* statement = nullptr;
    int returnCode = sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr); 
    if(returnCode != SQLITE_OK)
    {
        std::cout << "problem is in loading tournaments" << std::endl;
        std::cout << "cannot be converted into sqlite command" << std::endl;
        return false;
    }
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        const boost::uuids::uuid tid = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        const std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
        const int year = sqlite3_column_int(statement, 2);
        const SportsType type = Tools::sportsTypeStringToEnum(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3))));
        const TournamentSize size = Tools::sizeIntToEnum(sqlite3_column_int(statement, 4));
        const bool hasStarted = sqlite3_column_int(statement, 5) != 0;        
        const Round currentRound = Tools::roundStringToEnum(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 6)))); 
        Tournament t(tid, name, year, type, size, currentRound);
        t.setHasStarted(hasStarted);
        if (sqlite3_column_type(statement, 7) != SQLITE_NULL)
        {
            const boost::uuids::uuid winnerId = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 7)));
            for (Player& player : players)
            {
                if (player.getId() == winnerId)
                {
                    t.setWinner(player);
                    break;
                }
            }
        }
        tournaments.push_back(t);
        
    }
    sqlite3_finalize(statement); // Artık sorguyla işin bitti sorguyla is bitince kullandigi kaynaklari serbest birakir 
    return true;
     
}

bool DBManager::loadRankings(std::vector<Player>& players)
{
    if(players.empty())
    {
        std::cout << "players must be loaded to get the rankigs" << std::endl;
        return false;
    }
    const char* sqlCommand = "SELECT player_id, sport_type, points FROM player_rankings;";
    sqlite3_stmt* statement  = nullptr;
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
       std::cout << "failed to read load ranking statement" << std::endl;
       return false;
    }
    
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        const boost::uuids::uuid id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        const std::string type = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
        const int points = sqlite3_column_int(statement, 2);
        for (Player& player : players)
        {
            if (player.getId() == id)
            {
                player.addPoints(Tools::sportsTypeStringToEnum(type), points);
                break;
            }
        }
    }
    sqlite3_finalize(statement); // Artık sorguyla işin bitti sorguyla is bitince kullandigi kaynaklari serbest birakir 
    return true;    
}   




bool DBManager::loadMatches(std::vector<Player>& players, std::vector<Tournament>& tournaments)
{
    if(!(players.empty() && tournaments.empty()) && (players.empty() || tournaments.empty()))
    {
        std::cout << "players and tournaments should be loaded first" << std::endl;
        return false;
    }
    const char* sqlCommand = "SELECT id, tournament_id, home_player_id, away_player_id, round, home_score, away_score, is_played FROM matches";
    sqlite3_stmt* statement = nullptr;
    int returnCode = sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr);
    if(returnCode != SQLITE_OK)
    {
        std::cout << "failed to load match statement" << std::endl;
        return false;
    }
      
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        //round, home_score, away_score, is_played
        const boost::uuids::uuid id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        const boost::uuids::uuid tournament_id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
        const boost::uuids::uuid home_player_id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
        const boost::uuids::uuid away_player_id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)));
        const Round round =Tools::roundStringToEnum(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4))));
        const int homeScore = sqlite3_column_int(statement, 5);
        const int awayScore = sqlite3_column_int(statement, 6);
        bool hasStarted = sqlite3_column_int(statement, 7) != 0;
        Player* homePlayer = nullptr;
        Player* awayPlayer = nullptr;
        for (Player& player : players)
        {
            if (player.getId() == home_player_id)
            {
                homePlayer = &player;
            }
            else if(player.getId() == away_player_id)
            {
                awayPlayer = &player;
            }
        }
        Tournament* tournament = nullptr;
        for(Tournament& t: tournaments)
        {
            if(t.getId() == tournament_id)
            {
                tournament = &t;
            }
        }
        if (!homePlayer || !awayPlayer || !tournament || homePlayer == awayPlayer)
        {
            std::cerr << "Skipping match with missing/invalid participants: " << id << std::endl;
            continue;
        }
        Match m(id, *homePlayer, *awayPlayer, round);
        m.setScore(Score{homeScore, awayScore});
        tournament->addMatch(m);
    }
    sqlite3_finalize(statement); // Artık sorguyla işin bitti sorguyla is bitince kullandigi kaynaklari serbest birakir 
    return true;    
}

bool DBManager::loadTournamentPlayers(std::vector<Player>& players, std::vector<Tournament>& tournaments)
{
    if(!(players.empty() && tournaments.empty()) && (players.empty() || tournaments.empty()))
    {
        std::cout << "players and tournaments must be loaded before loading tournament_players" << std::endl;
        return false;
    }
    //butun tournamentler icin playerIdleri donecek 
    const char* sqlCommand = "SELECT tournament_id, player_id FROM tournament_players";
    sqlite3_stmt* statement = nullptr;
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr)  != SQLITE_OK)
    {
        std::cout << "lalalal" << std::endl;
        std::cout << "could not prepare statement" << std::endl;
        std::cout  << sqlite3_errmsg(dbPointer) << std::endl;
        return false;
    }
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        const boost::uuids::uuid tournament_id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        const boost::uuids::uuid player_id = boost::uuids::string_generator{}(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
        for(Tournament& t: tournaments)
        {
            for(Player& p:players)
            {
                if(t.getId() == tournament_id && p.getId() == player_id)
                {
                    t.addPlayer(p);
                }
            }
        }
    }
    sqlite3_finalize(statement);
    return true; 
}

//bence degistirilmesi gerek
void DBManager::loadAll(std::vector<Player>& players, std::vector<Tournament>& tournaments)
{
    loadPlayers(players);
    loadTournaments(tournaments, players);
    loadRankings(players);
    loadTournamentPlayers( players, tournaments);
    loadMatches(players,tournaments);
}
