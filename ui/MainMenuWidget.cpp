#include "MainMenuWidget.h"

#include <QLabel> 
#include <QPushButton>
#include <QBoxLayout>
#include <QGridLayout>

MainMenuWidget::MainMenuWidget(QWidget* parent)
    :QWidget(parent)
{
    auto* layout = new QVBoxLayout(this); //vertical box layout 
    layout->setContentsMargins(60,50,60,50);
    layout->setSpacing(16); // layout arasindaki ogelerin arasindaki bosluk

    auto* title = new QLabel("MatchForge", this);
    title->setObjectName("title");

    auto* subtitle = new QLabel("Main menu", this);
    subtitle->setObjectName("subtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(24);

    auto* buttons = new QGridLayout(); // satir ve sutun seklinde gridlere boluyor 
    buttons->setSpacing(18);

    auto* createPlayer = new QPushButton("create player", this);
    connect(createPlayer, &QPushButton::clicked, this, &MainMenuWidget::createPlayerRequested);
    auto* createTournament = new QPushButton("create tournament", this);
    connect(createTournament, &QPushButton::clicked, this, &MainMenuWidget::createTournamentRequested);
    auto* searchPlayer = new QPushButton("search player", this);
    connect(searchPlayer, &QPushButton::clicked, this, &MainMenuWidget::searchPlayerRequested);
    auto* searchTournament = new QPushButton("search tournament", this);
    connect(searchTournament, &QPushButton::clicked, this, &MainMenuWidget::searchTournamentRequested);
    auto* leaderBoard = new QPushButton("view leaderboard", this);
    connect(leaderBoard, &QPushButton::clicked, this, &MainMenuWidget::leaderBoardRequested);
    auto* exitButton = new QPushButton("Exit", this);
    connect(exitButton, &QPushButton::clicked, this, &MainMenuWidget::exitRequested);

    createPlayer->setObjectName("primary");

    buttons->addWidget(createPlayer, 0, 0);
    buttons->addWidget(createTournament, 0, 1);
    buttons->addWidget(searchPlayer, 1, 0);
    buttons->addWidget(searchTournament, 1, 1);
    buttons->addWidget(leaderBoard, 2, 0);
    buttons->addWidget(exitButton, 2, 1);

    buttons->setColumnStretch(0, 1); //pencere buyuutulduugunde hangi buton ne kadar genislesin, sol sutun 1 pay
    buttons->setColumnStretch(1, 1);

    layout->addLayout(buttons);
    layout->addStretch();

}
