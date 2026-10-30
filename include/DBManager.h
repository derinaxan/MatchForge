#pragma once

#include <string>
#include <vector>

#include "Player.h"
#include "Tournament.h"
#include "sqlite3.h"
#include "Tools.h"
#include "Ranking.h"

class DBManager
{
private:
    sqlite3* dbPointer = nullptr; 
    std::string connectionString; 
    DBManager(std::string& connectionString);
    bool executeSQL(const std::string& sqlCommand);
    
    bool createPlayersTable();
    bool loadPlayers(std::vector<Player>& players);
    bool insertPlayer(const Player& player);
    bool updatePlayer(const Player& player);
    bool playerExists(const boost::uuids::uuid& playerId);
    void savePlayers(const std::vector<Player>& players);
 
    bool createTournamentsTable();
    bool loadTournaments(std::vector<Tournament>& tournaments, std::vector<Player>& players);
    bool insertTournament(const Tournament& tournament);
    bool updateTournament(const Tournament& tournament); 
    bool tournamentExists(const boost::uuids::uuid tournamentExists);
    void saveTournaments(const std::vector<Tournament>& tournaments);

    bool createMatchesTable();
    bool loadMatches(std::vector<Player>& players, std::vector<Tournament>& tournaments);
    bool insertMatch(const Match& match, const boost::uuids::uuid& tournamentId);
    bool updateMatch(const Match& match, const boost::uuids::uuid& tournamentId);
    bool matchExists(const boost::uuids::uuid matchId);
    void saveMatches(const std::vector<Tournament>& tournmanets);

    bool craetePlayerRankingsTable();
    bool loadRankings(std::vector<Player>& players);
    bool insertRanking( const boost::uuids::uuid& playerId, const Ranking& ranking);
    bool updateRanking( const boost::uuids::uuid& playerId, const Ranking& ranking);
    bool rankingExists(const boost::uuids::uuid& playerId, const SportsType& sportsType);
    void saveRankings(const std::vector<Player>& players);

    bool createTournamentPlayersTable();
    bool loadTournamentPlayers(std::vector<Player>& players, std::vector<Tournament>& tournaments);
    bool insertTournamentPlayer( const boost::uuids::uuid& tournamentId,const boost::uuids::uuid& playerId);
    bool tournamentPlayerExists(const boost::uuids::uuid tournamentId, const boost::uuids::uuid playerId);
    void saveTournamentPlayers(const std::vector<Tournament>& tournaments);
    

public:
    DBManager(const std::string& connectionString);
    void connect();
    bool createTables();
    void loadAll(std::vector<Player>& players, std::vector<Tournament>& tournaments);
    void saveAll(const std::vector<Player>& players, const std::vector<Tournament>& tournaments);
    void disconnect(); // gerek var mi emin degilim 
};