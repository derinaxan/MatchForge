cmake_minimum_required(VERSION 3.20)

project(MatchForge)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(MatchForge
    src/main.cpp
    src/MainManager.cpp 
    src/Match.cpp 
    src/Menu.cpp 
    src/Player.cpp 
    src/PlayerService.cpp 
    src/Round.cpp
    src/Tools.cpp 
    src/Tournament.cpp 
    src/TournamentService.cpp
    src/database/DBManager.cpp
    src/database/DBManagerLoad.cpp 
    src/database/DBManagerSave.cpp
    external/sqlite3.c
    ui/MainWindow.cpp
    ui/MainMenuWidget.cpp
    ui/CreatePlayerWidget.cpp
    ui/CreateTournamentWidget.cpp
    ui/SearchPlayerWidget.cpp
    ui/PlayerDetailsWidget.cpp
    ui/LeaderBoardWidget.cpp 
    ui/SearchTournamentWidget.cpp 
    ui/TournamentDetailsWidget.cpp
)

#projene calisirken includelanacak directoryler 
target_include_directories(MatchForge PRIVATE 
    include 
    external)

target_link_libraries(MatchForge PRIVATE Qt6::Widgets)