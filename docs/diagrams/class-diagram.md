# Class diagram

The Qt main window owns a single manager. Both services work with the collections owned by that manager.

```mermaid
classDiagram
    direction TB
    MainWindow *-- MainManager : manager
    MainManager *-- PlayerService
    MainManager *-- TournamentService
    MainManager *-- DBManager
    MainManager *-- "0..*" Player : players
    MainManager *-- "0..*" Tournament : tournaments
    PlayerService ..> Player : searches and creates
    TournamentService ..> Tournament : starts and advances
    Tournament *-- "0..*" Match : matches
    Tournament --> "0..*" Player : participants
    Tournament --> "0..1" Player : winner
    Match --> "1" Player : homePlayer
    Match --> "1" Player : awayPlayer
    Match *-- Score
    Player *-- "0..*" Ranking
    class DBManager {
        +connect()
        +createTables()
        +loadAll()
        +saveAll()
        +disconnect()
    }
    class Tournament {
        -uuid id
        -string name
        -int year
        -bool hasStarted
        -SportsType type
        -Round currentRound
        -TournamentSize size
    }
    class Player {
        -uuid id
        -string name
        -string surname
        -int strength
    }
    class Match {
        -uuid id
        -Round round
    }
    class Score {
        +int homeScore
        +int awayScore
    }
    class Ranking {
        +SportsType type
        +int points
    }
```

A filled diamond marks ownership. A tournament stores its participants as `std::vector<Player*>` and its winner as a nullable `Player*`. Each match holds two player references. Scores and rankings are stored by value.

[Browse the C++ declarations](../../include)
