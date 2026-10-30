#include "MainManager.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>

MainManager::MainManager()
    :tournamentService(tournaments),
    playerService(players), 
    dbManager("data/matchForge.db")
    {}



MainManager::~MainManager()
{
    if(initilzed)
    {
        dbManager.disconnect();
    }
}

void MainManager::initilize()
{
    if(initilzed) return;
    dbManager.connect();
    if(!dbManager.createTables()) throw std::runtime_error("database initilazation failed");
    initilzed = true;
    dbManager.loadAll(players, tournaments);
}

void MainManager::save()
{
    if(!initilzed) throw std::runtime_error("database not initilized");
    dbManager.saveAll(players, tournaments);
}

void MainManager::createPlayer(std::string& name, std::string& surname, int strength)
{
    if (players.size() < players.capacity())
    {
        playerService.createPLayer(name, surname, strength);
        return;
    }

    // Keep vectors, but rebuild their cross-references before changing storage.
    // Work in temporary containers so allocation failures leave the model intact.
    std::unordered_map<const Player*, std::size_t> indices;
    for (std::size_t i = 0; i < players.size(); ++i) indices.emplace(&players[i], i);
    std::vector<Player> grownPlayers = players;
    grownPlayers.reserve(std::max<std::size_t>(1, players.capacity() * 2));
    PlayerService growingService(grownPlayers);
    growingService.createPLayer(name, surname, strength);
    auto rebound = [&](const Player* player) -> Player&
    {
        const auto found = indices.find(player);
        if (found == indices.end()) throw std::runtime_error("Tournament has an invalid player reference.");
        return grownPlayers[found->second];
    };
    std::vector<Tournament> reboundTournaments;
    reboundTournaments.reserve(tournaments.size());
    for (const Tournament& old : tournaments)
    {
        Tournament rebuilt(old.getId(), old.getName(), old.getYear(), old.getType(), old.getSize(), old.getCurrentRound());
        rebuilt.setHasStarted(old.getHasStarted());
        for (const Player* player : old.getPlayers()) rebuilt.addPlayer(rebound(player));
        if (old.getWinner()) rebuilt.setWinner(rebound(old.getWinner()));
        for (const Match& match : old.getMatches())
        {
            Match rebuiltMatch(match.getId(), rebound(&match.getHomePlayer()), rebound(&match.getAwayPlayer()), match.getRound());
            rebuiltMatch.setScore(match.getScore());
            rebuilt.addMatch(rebuiltMatch);
        }
        reboundTournaments.push_back(std::move(rebuilt));
    }
    players.swap(grownPlayers);
    tournaments.swap(reboundTournaments);
}

std::vector<Player*> MainManager::searchPlayer(std::string& name, std::string& surname)
{
    return playerService.getMathcingPlayersByNameSurname(name, surname);
}

void MainManager::createTournament(std::string& name, int year, SportsType type, TournamentSize size)
{
    tournamentService.createTournament(name, year, type, size);
}

std::vector<Tournament*> MainManager::searchTournament(std::string& name)
{
   return tournamentService.getTournamentByName(name);
}

bool MainManager::addPlayerToTournament(Player* player, Tournament* tournament)
{
    if(player == nullptr || tournament == nullptr) return false;
    if(tournament->getHasStarted()) return false;
    if(tournament->getPlayers().size() >= Tools::sizeEnumToInt(tournament->getSize())) return false;
    return tournament->addPlayer(*player);
}

bool MainManager::removePlayerFromTournament(Player* player, Tournament* tournament)
{
    if(player == nullptr || tournament == nullptr) return false;
    if(tournament->getHasStarted()) return false;
    return tournament->removePlayer(*player);
}

std::vector<Tournament*> MainManager::getPlayedTournaments(Player& player)
{
    std::vector<Tournament*> result;
    for(Tournament& t: tournaments)
    {
        if(t.containsPlayer(player))
        {
            result.push_back(&t);
        }
    }    
    return result; 
}

std::vector<Tournament*> MainManager::getNonPlayedTournaments(Player& player)
{
    std::vector<Tournament*> result;
    for(Tournament& t: tournaments)
    {
        if(!t.containsPlayer(player))
        {
            result.push_back(&t);
        }
    }
    return result;
}

void MainManager::startTournament(Tournament& tournament)
{
    tournamentService.start(tournament);
}

void MainManager::simulateRound(Tournament& t)
{
    tournamentService.simulateRound(t);
}

std::vector<Player> MainManager::getLeaderBoard(SportsType type) const
{
    std::vector<Player> result = players;
    std::sort(result.begin(), result.end(),  [type](const Player& first, const Player& second)
        {
            return first.getPoints(type) > second.getPoints(type);
        });
    if(result.size() > 5)
    {
        result.erase(result.begin() +5, result.end());
    }
    return result;
}   




