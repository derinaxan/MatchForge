#include "DBManager.h"
#include "Tools.h"
#include <stdexcept>
#include <unordered_set>

namespace
{
struct FinalizeStatement
{
    sqlite3_stmt*& statement;
    ~FinalizeStatement() { sqlite3_finalize(statement); }
};
void requireSaved(bool success, sqlite3* database)
{
    if (!success) throw std::runtime_error(std::string("Could not save changes: ") + sqlite3_errmsg(database));
}
}

#include <boost/uuid/uuid.hpp>             // uuid tipi
#include <boost/uuid/uuid_generators.hpp>  // random_generator
#include <boost/uuid/string_generator.hpp> // string -> UUID
#include <boost/uuid/uuid_io.hpp>          // UUID -> string

bool DBManager::insertPlayer(const Player& player)
{
    const char* sqlCommand = "INSERT INTO players(id, name, surname, strength) VALUES(?,?,?,?);";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "could not prepare statement for insert player" << std::endl;
        return false ;
    }
    std::string id = boost::uuids::to_string(player.getId());
    std::string name = player.getName();
    std::string surname = player.getSurname();
    bool bound = (sqlite3_bind_text(statement, 1, id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 2, name.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 3, surname.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_int(statement, 4, player.getStrength()) == SQLITE_OK); 
    if(!bound)
    {
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 1 << std::endl;
        std::cout << sqlite3_errmsg(dbPointer) << std::endl;
        std:: cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::updatePlayer(const Player& player)
{
    const char* sql = "UPDATE players SET name = ?, surname = ?, strength = ? WHERE id = ? AND (name IS NOT ?1 OR surname IS NOT ?2 OR strength IS NOT ?3);"; 
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sql, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "updatePlayer error" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string id = boost::uuids::to_string(player.getId());
    std::string name = player.getName();
    std::string surname = player.getSurname();
    bool bound = (sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) &&
    (sqlite3_bind_text(statement, 2, surname.c_str(), -1, SQLITE_TRANSIENT)  == SQLITE_OK) &&
    (sqlite3_bind_int(statement, 3, player.getStrength()) == SQLITE_OK) && 
    (sqlite3_bind_text(statement, 4, id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "could not bind statment" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 2 << std::endl;
        std::cout << sqlite3_errmsg(dbPointer) << std::endl;
        std::cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}


bool DBManager::playerExists(const boost::uuids::uuid& playerId)
{
    const char* sqlCommand = "SELECT 1 FROM players WHERE id=?;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "exists Player error" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false; 
    }
    std::string player_id = boost::uuids::to_string(playerId);
    if(sqlite3_bind_text(statement, 1, player_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cout << "could not bind statement" << std::endl;
        return false;
    }
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
    
        return false;
    }


    return true;
}


void DBManager::savePlayers(const std::vector<Player>& players)
{
    for(const Player& p : players)
    {
        if(!playerExists(p.getId()))
        {
            requireSaved(insertPlayer(p), dbPointer);
        }
        else
        {
            requireSaved(updatePlayer(p), dbPointer);
        }
    }
}

bool DBManager::insertTournament(const Tournament& tournament)
{
    const char* sqlCommand = "INSERT INTO tournaments(id, name, year, sport_type, size, has_started, current_round, winner_id) VALUES(?,?,?,?,?,?,?,?);";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "Sikinti burada" << std::endl;
        std::cout << sqlite3_errmsg(dbPointer) << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string id = boost::uuids::to_string(tournament.getId());
    std::string name = tournament.getName();
    std::string sports_type = Tools::sportsTypeEnumToString(tournament.getType());
    int size = Tools::sizeEnumToInt(tournament.getSize());
    int hasStarted = tournament.getHasStarted() ? 1 : 0;
    std::string currentRound = Tools::roundEnumToString(tournament.getCurrentRound());
    bool bound = 
    (sqlite3_bind_text(statement, 1, id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 2, name.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_int(statement, 3, tournament.getYear()) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 4, sports_type.c_str(),-1,  SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_int(statement, 5, size) == SQLITE_OK) && (sqlite3_bind_int(statement, 6, hasStarted) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 7, currentRound.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    if(tournament.getWinner() != nullptr)
    {
        std::string winnerId = boost::uuids::to_string(tournament.getWinner()->getId());
      (sqlite3_bind_text(statement, 8, winnerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    }
    if(!bound)
    {  
        std::cout << sqlite3_errmsg(dbPointer) << std::endl;
        std::cout << "sikinti baba" << std::endl;
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 3 << std::endl;
        std:: cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::updateTournament(const Tournament& tournament)
{
    const char* sqlCommand = "UPDATE tournaments SET name = ?, year = ?, sport_type = ?, size =?, has_started = ?, current_round =?, winner_id = ? WHERE id = ? AND (name IS NOT ?1 OR year IS NOT ?2 OR sport_type IS NOT ?3 OR size IS NOT ?4 OR has_started IS NOT ?5 OR current_round IS NOT ?6 OR winner_id IS NOT ?7);";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "error in update tournament" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string name = tournament.getName();
    std::string sports_type = Tools::sportsTypeEnumToString(tournament.getType());
    int size = Tools::sizeEnumToInt(tournament.getSize());
    int hasStarted = tournament.getHasStarted() ? 1 : 0;
    std::string currentRound = Tools::roundEnumToString(tournament.getCurrentRound());
    bool bound = 
    (sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_int(statement, 2, tournament.getYear()) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 3, sports_type.c_str(),-1,  SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_int(statement, 4, size) == SQLITE_OK) && (sqlite3_bind_int(statement, 5, hasStarted) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 6, currentRound.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 8, boost::uuids::to_string(tournament.getId()).c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    if(tournament.getWinner() != nullptr)
    {
        std::string winnerId = boost::uuids::to_string(tournament.getWinner()->getId());
      (sqlite3_bind_text(statement, 7, winnerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    }
    if(!bound)
    {
        std::cout << "update dournament bind" << std::endl;
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 4 << std::endl;
        std:: cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::tournamentExists(const boost::uuids::uuid tournamentId)
{
    const char* sqlCommand = "SELECT 1 FROM tournaments WHERE id=?;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "error in tournament exists" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false; 
    }
    std::string tournament_id = boost::uuids::to_string(tournamentId);
    if(sqlite3_bind_text(statement, 1, tournament_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cout << "could not bind statement" << std::endl;
        return false;
    }
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
    
        return false;
    }


    return true;
}


void  DBManager::saveTournaments(const std::vector<Tournament>& tournaments)
{
    for(const Tournament& t: tournaments)
    {
        if(tournamentExists(t.getId()))
        {
            requireSaved(updateTournament(t), dbPointer);
        }
        else{
            requireSaved(insertTournament(t), dbPointer);
        }
    }
}

bool DBManager::insertMatch(const Match& match, const boost::uuids::uuid& tournamentId)
{
    const char* sqlCommand = "INSERT INTO matches(id, tournament_id, home_player_id, away_player_id, round, home_score, away_score) VALUES(?,?,?,?,?,?,?);";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "error in match insertion" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string id = boost::uuids::to_string(match.getId());
    std::string tournament_id = boost::uuids::to_string(tournamentId);
    std::string homePlayerId = boost::uuids::to_string(match.getHomePlayer().getId());
    std::string awayPlayerId = boost::uuids::to_string(match.getAwayPlayer().getId());
    std::string round = Tools::roundEnumToString(match.getRound());
    bool bound = (sqlite3_bind_text(statement, 1, id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 2, tournament_id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 3, homePlayerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 4, awayPlayerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 5, round.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_int(statement, 6, match.getScore().homeScore) == SQLITE_OK) && (sqlite3_bind_int(statement, 7, match.getScore().awayScore) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "sikinti burada match insertion" << std::endl;
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 5 << std::endl;
        std:: cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::updateMatch(const Match& match, const boost::uuids::uuid& tournamentId)
{
    const char* sqlCommand = "UPDATE matches SET tournament_id = ?, home_player_id = ?, away_player_id = ? , round =? , home_score = ? , away_score =? WHERE id = ? AND (tournament_id IS NOT ?1 OR home_player_id IS NOT ?2 OR away_player_id IS NOT ?3 OR round IS NOT ?4 OR home_score IS NOT ?5 OR away_score IS NOT ?6) ";
    
      sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "problem in update Match" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string id = boost::uuids::to_string(match.getId());
    std::string tournament_id = boost::uuids::to_string(tournamentId);
    std::string homePlayerId = boost::uuids::to_string(match.getHomePlayer().getId());
    std::string awayPlayerId = boost::uuids::to_string(match.getAwayPlayer().getId());
    std::string round = Tools::roundEnumToString(match.getRound());
    bool bound =   (sqlite3_bind_text(statement, 1, tournament_id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK) 
    && (sqlite3_bind_text(statement, 2, homePlayerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 3, awayPlayerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 4, round.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_int(statement, 5, match.getScore().homeScore) ==SQLITE_OK) && (sqlite3_bind_int(statement, 6, match.getScore().awayScore) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 7, id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << 6 << std::endl;
        std:: cout << "could not execute statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::matchExists(const boost::uuids::uuid matchId)
{
    const char* sqlCommand = "SELECT 1 FROM matches WHERE id=?;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "Problem in match exists" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false; 
    }
    std::string match_id = boost::uuids::to_string(matchId);
    if(sqlite3_bind_text(statement, 1, match_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cout << "could not bind statement" << std::endl;
        return false;
    }
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
    
        return false;
    }


    return true;
}

void DBManager::saveMatches(const std::vector<Tournament>& tournaments)
{
    for(const Tournament& t: tournaments)
    {
        for(const Match& m: t.getMatches())
        {
            if(matchExists(m.getId()))
            {
                requireSaved(updateMatch(m, t.getId()), dbPointer);
            }
            else
            {
                requireSaved(insertMatch(m, t.getId()), dbPointer);
            }
        }
        
    }
}

bool DBManager::insertRanking( const boost::uuids::uuid& playerId, const Ranking& ranking)
{
    const char* sqlCommand = "INSERT INTO player_rankings(player_id, sport_type, points) VALUES(?,?,?);"; 
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "Problem in insert ranknig" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false;
    }
    std::string player_id = boost::uuids::to_string(playerId);
    std::string sports_type = Tools::sportsTypeEnumToString(ranking.type);
    bool bound = (sqlite3_bind_text(statement, 1, player_id.c_str(), -1, SQLITE_TRANSIENT)  == SQLITE_OK)
    && (sqlite3_bind_text(statement, 2, sports_type.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_int(statement, 3, ranking.points) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "could not bind the statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << "cout not execut statemtn" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::updateRanking( const boost::uuids::uuid& playerId, const Ranking& ranking)
{
    const char* sqlCommand = "UPDATE player_rankings SET points = ? WHERE player_id =? AND sport_type = ? AND points IS NOT ?1;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "problem in update ranking" << std::endl;
        std::cout << "could not prepare statement" << std::endl;
        return false;
    }
    std::string player_id  = boost::uuids::to_string(playerId);
    std::string sports_type = Tools::sportsTypeEnumToString(ranking.type);
    bool bound = (sqlite3_bind_int(statement, 1, ranking.points) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 2, player_id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 3, sports_type.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "statement cannot be bound" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE) 
    {
        std::cout << "cannot execut statement" << std::endl;
        return false;
    }

    return true;
}

bool DBManager::rankingExists(const boost::uuids::uuid& playerId, const SportsType& sportsType)
{
    const char* sqlCommand = "SELECT 1 FROM player_rankings WHERE player_id=? AND sport_type = ?;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "problem in ranking exists" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false; 
    }
    std::string player_id = boost::uuids::to_string(playerId);
    if(sqlite3_bind_text(statement, 1, player_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK  || 
    sqlite3_bind_text(statement, 2, Tools::sportsTypeEnumToString(sportsType).c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK )
    {
        std::cout << "could not bind statement" << std::endl;
        return false;
    }
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
    
        return false;
    }


    return true;
}

void DBManager::saveRankings(const std::vector<Player>& players)
{
    for(const Player& p:players)
    {
        for(const Ranking& ranking : p.getRankings())
        {
            if(rankingExists(p.getId(), ranking.type))
            {
                requireSaved(updateRanking(p.getId(), ranking), dbPointer);
            }
            else
            {
                requireSaved(insertRanking(p.getId(), ranking), dbPointer);
            }
        }
    }
    

}
bool DBManager::insertTournamentPlayer( const boost::uuids::uuid& tournamentId,const boost::uuids::uuid& playerId)
{
    const char* sqlCommand = "INSERT INTO tournament_players(tournament_id, player_id) VALUES(?,?);";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement}; 
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "problem insertTournamentPlayer" << std::endl;
        std::cout << "could not prepare statement" << std::endl;
        return false;
    }
    std::string tournament_id = boost::uuids::to_string(tournamentId);
    std::string player_id = boost::uuids::to_string(playerId);
    bool bound = (sqlite3_bind_text(statement, 1, tournament_id.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK)
    && (sqlite3_bind_text(statement, 2 , player_id.c_str(), -1 , SQLITE_TRANSIENT) == SQLITE_OK);
    if(!bound)
    {
        std::cout << "cannot bind insertTournamentPlayer statement" << std::endl;
        return false;
    }
    if(sqlite3_step(statement) != SQLITE_DONE)
    {
        std::cout << "cannot execute statemtn" << std::endl;
        return false;
    }
    return true;
}


bool DBManager::tournamentPlayerExists(const boost::uuids::uuid tournamentId, const boost::uuids::uuid playerId)
{
    const char* sqlCommand = "SELECT 1 FROM tournament_players WHERE tournament_id=? AND player_id = ?;";
    sqlite3_stmt* statement = nullptr;
    FinalizeStatement cleanup{statement};
    if(sqlite3_prepare_v2(dbPointer, sqlCommand, -1, &statement, nullptr) != SQLITE_OK)
    {
        std::cout << "problem tournament player exists" << std::endl;
        std::cout << "could not prepare the statement" << std::endl;
        return false; 
    }
    std::string tournament_id = boost::uuids::to_string(tournamentId);
    std::string player_id = boost::uuids::to_string(playerId);

    if(sqlite3_bind_text(statement, 1, tournament_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
    sqlite3_bind_text(statement, 2, player_id.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
    {
        std::cout << "could not bind statement" << std::endl;
        return false;
    }
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
    
        return false;
    }


    return true;
}

void DBManager::saveTournamentPlayers(const std::vector<Tournament>& tournaments)
{
    for(const Tournament& tournament: tournaments)
    {
        const std::string tournamentId = boost::uuids::to_string(tournament.getId());
        std::unordered_set<std::string> currentPlayers;
        for (const Player* player : tournament.getPlayers()) currentPlayers.insert(boost::uuids::to_string(player->getId()));
        std::vector<std::string> removed;
        {
            sqlite3_stmt* statement = nullptr;
            FinalizeStatement cleanup{statement};
            requireSaved(sqlite3_prepare_v2(dbPointer, "SELECT player_id FROM tournament_players WHERE tournament_id=? AND player_id IN (SELECT id FROM players);", -1, &statement, nullptr) == SQLITE_OK, dbPointer);
            requireSaved(sqlite3_bind_text(statement, 1, tournamentId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK, dbPointer);
            int result;
            while ((result = sqlite3_step(statement)) == SQLITE_ROW)
            {
                const std::string id(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
                if (!currentPlayers.contains(id)) removed.push_back(id);
            }
            requireSaved(result == SQLITE_DONE, dbPointer);
        }
        for (const auto& playerId : removed)
        {
            sqlite3_stmt* statement = nullptr;
            FinalizeStatement cleanup{statement};
            requireSaved(sqlite3_prepare_v2(dbPointer, "DELETE FROM tournament_players WHERE tournament_id=? AND player_id=?;", -1, &statement, nullptr) == SQLITE_OK, dbPointer);
            requireSaved(sqlite3_bind_text(statement, 1, tournamentId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK, dbPointer);
            requireSaved(sqlite3_bind_text(statement, 2, playerId.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK, dbPointer);
            requireSaved(sqlite3_step(statement) == SQLITE_DONE, dbPointer);
        }
        for(const Player* p: tournament.getPlayers())
        {
            if(!tournamentPlayerExists(tournament.getId(), p->getId()))
            {
                requireSaved(insertTournamentPlayer(tournament.getId(), p->getId()), dbPointer);
            }
        }
    }
}


void DBManager::saveAll(const std::vector<Player>& players, const std::vector<Tournament>& tournaments)
{
    requireSaved(executeSQL("BEGIN IMMEDIATE;"), dbPointer);
    try
    {
        savePlayers(players);
        saveTournaments(tournaments);
        saveMatches(tournaments);
        saveRankings(players);
        saveTournamentPlayers(tournaments);
        requireSaved(executeSQL("COMMIT;"), dbPointer);
    }
    catch (...)
    {
        executeSQL("ROLLBACK;");
        throw;
    }
}