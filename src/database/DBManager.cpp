#include "DBManager.h"


DBManager::DBManager(const std::string& connectionString)
    :connectionString(connectionString)
    {}

bool DBManager::executeSQL(const std::string& sqlCommand)
{
    char* error_message= nullptr;
    int returnCode = sqlite3_exec(dbPointer, sqlCommand.c_str(), nullptr, nullptr, &error_message);
    if(returnCode != SQLITE_OK) 
    {
        std::cout << "could not execute sql command" << sqlCommand << std::endl;
        sqlite3_free(error_message); // hata mesaji icin ayrilan bellegi serbest birak
        return false;
    }
    return true;
}


bool DBManager::createPlayersTable()
{
    std::string createPlayersSql = "CREATE TABLE IF NOT EXISTS players("
    "id TEXT PRIMARY KEY,"
    "name TEXT NOT NULL," 
    "surname TEXT NOT NULL,"
    "strength int NOT NULL"
    ")"; 
    if(!executeSQL(createPlayersSql))
    {
        std::cout << "could not create players table" << std::endl;
        sqlite3_close(dbPointer);
        return false;
    }
    return true;
}

bool DBManager::craetePlayerRankingsTable()
{
    std::string createPlayerRankings = "CREATE TABLE IF NOT EXISTS player_rankings("
    "player_id TEXT REFERENCES players(id),"
    "sport_type TEXT NOT NULL,"
    "points INT NOT NULL DEFAULT 0,"
    "PRIMARY KEY(player_id, sport_type)"
    ")";
    if(!executeSQL(createPlayerRankings))
    {
        std::cout << "could not create players rankings table" << std::endl;
        sqlite3_close(dbPointer);
        return false;
    }
    std:: cout << "craeted player rankings table" << std::endl;
    return true;
}

bool DBManager::createTournamentsTable()
{
    std::string createTournamentSQL = "CREATE TABLE IF NOT EXISTS tournaments("
    "id TEXT PRIMARY KEY,"
    "name TEXT NOT NULL,"
    "year INT NOT NULL,"
    "sport_type TEXT NOT NULL,"
    "size INT NOT NULL,"
    "has_started INTEGER NOT NULL DEFAULT 0 CHECK(has_started IN (0, 1)),"
    "current_round TEXT NOT NULL,"
    "winner_id TEXT REFERENCES players(id)"    
    ")";
    //one to many ilisklilerde one olan tarafi manynin icine yazariz 
    if(!executeSQL(createTournamentSQL))
    {
        std::cout << "could not craete tournaments table" << std::endl;
        sqlite3_close(dbPointer);
        return false;
    }
    return true;
}

bool DBManager::createMatchesTable()
{
    std::string createMathcesSQL = "CREATE TABLE IF NOT EXISTS matches("
    "id TEXT PRIMARY KEY,"
    "tournament_id TEXT REFERENCES tournaments(id) NOT NULL,"
    "home_player_id TEXT REFERENCES players(id),"
    "away_player_id TEXT REFERENCES players(id),"
    "round TEXT NOT NULL,"
    "home_score INTEGER,"
    "away_score INTEGER,"
    "is_played INTEGER NOT NULL DEFAULT 0 CHECK(is_played IN(0,1))"
    ")";
    if(!executeSQL(createMathcesSQL))
    {
        std::cout << "coudl not create matches table" << std::endl;
        sqlite3_close(dbPointer); // database baglantisni kapat
        return false;
    }
    return true;

}

bool DBManager::createTournamentPlayersTable()
{
    std::string creatTournamentPlayers = "CREATE TABLE IF NOT EXISTS tournament_players("
    "tournament_id TEXT NOT NULL REFERENCES tournaments(id),"
    "player_id TEXT NOT NULL REFERENCES players(id),"
    "PRIMARY KEY(tournament_id, player_id)"
    ")";
    if(!executeSQL(creatTournamentPlayers))
    {
        std::cout << "could not create tournament_players table" << std::endl;
        return false;
    }
    return true;
};



void DBManager::connect()
{
    int returnCode = sqlite3_open(connectionString.c_str(), &dbPointer);
    if(returnCode != SQLITE_OK)
    {
        std::cout << "database connection cannot be made" << std::endl;
        return;
    }
    std::cout << "database connected" << std::endl;
};



bool DBManager::createTables()
{
    return createPlayersTable() &&
    createMatchesTable() &&
    craetePlayerRankingsTable() &&
    createTournamentsTable() &&
    createTournamentPlayersTable();
};





void DBManager::disconnect()
{
    if(dbPointer != nullptr)
    {
        sqlite3_close(dbPointer);
        return;
    }
    std::cout << "database connection not open so cannot be closed" << std::endl;
}
