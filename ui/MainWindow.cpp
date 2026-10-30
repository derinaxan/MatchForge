#include "MainWindow.h"
#include "MainMenuWidget.h"
#include "CreatePlayerWidget.h"
#include "CreateTournamentWidget.h"
#include "SearchPlayerWidget.h"
#include "SearchTournamentWidget.h"
#include "PlayerDetailsWidget.h"
#include "TournamentDetailsWidget.h"
#include "LeaderBoardWidget.h"
#include <QMessageBox>
#include <exception>
#include <QStackedWidget>
#include <QCoreApplication>
#include <QDir>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("MatchForge");
    resize(1100, 700);
    auto* pages = new QStackedWidget(this);
    setCentralWidget(pages);

    // The existing backend uses a relative data/matchForge.db path.
    if (!QDir("data").exists())
    {
        QDir directory(QCoreApplication::applicationDirPath());
        do
        {
            if (directory.exists("data"))
            {
                QDir::setCurrent(directory.absolutePath());
                break;
            }
        } while (directory.cdUp());
    }
    try
    {
        manager.initilize();
    }
    catch (const std::exception& error)
    {
        QMessageBox::critical(this, "Open MatchForge", QString::fromUtf8(error.what()));
        pages->setEnabled(false);
        return;
    }

    auto* mainMenu = new MainMenuWidget(pages);
    auto* createPlayerPage = new CreatePlayerWidget(manager, pages);
    auto* createTournamentPage = new CreateTournamentWidget(manager, pages);
    auto* searchPlayerPage = new SearchPlayerWidget(manager, pages);
    auto* searchTournamentPage = new SearchTournamentWidget(manager, pages);
    auto* playerDetailsPage = new PlayerDetailsWidget(manager, pages);
    auto* tournamentDetailsPage = new TournamentDetailsWidget(manager, pages);
    auto* leaderBoardPage = new LeaderBoardWidget(manager, pages);

    for (QWidget* page : std::initializer_list<QWidget*>{mainMenu, createPlayerPage,
         createTournamentPage, searchPlayerPage, searchTournamentPage,
         playerDetailsPage, tournamentDetailsPage, leaderBoardPage})
        pages->addWidget(page);

    connect(mainMenu, &MainMenuWidget::createPlayerRequested, this, [=] { pages->setCurrentWidget(createPlayerPage); });
    connect(mainMenu, &MainMenuWidget::createTournamentRequested, this, [=] { pages->setCurrentWidget(createTournamentPage); });
    connect(mainMenu, &MainMenuWidget::searchPlayerRequested, this, [=]
    {
        searchPlayerPage->clearResults();
        pages->setCurrentWidget(searchPlayerPage);
    });
    connect(mainMenu, &MainMenuWidget::searchTournamentRequested, this, [=]
    {
        searchTournamentPage->clearResults();
        pages->setCurrentWidget(searchTournamentPage);
    });
    connect(mainMenu, &MainMenuWidget::leaderBoardRequested, this, [=]
    {
        if (leaderBoardPage->refresh()) pages->setCurrentWidget(leaderBoardPage);
    });
    connect(mainMenu, &MainMenuWidget::exitRequested, this, &QWidget::close);

    const auto showMenu = [=] { pages->setCurrentWidget(mainMenu); };
    connect(createPlayerPage, &CreatePlayerWidget::backRequested, this, showMenu);
    connect(createPlayerPage, &CreatePlayerWidget::playerCreated, this, showMenu);
    connect(createTournamentPage, &CreateTournamentWidget::backRequested, this, showMenu);
    connect(createTournamentPage, &CreateTournamentWidget::tournamentCreated, this, showMenu);
    connect(searchPlayerPage, &SearchPlayerWidget::backRequested, this, showMenu);
    connect(searchTournamentPage, &SearchTournamentWidget::backRequested, this, showMenu);
    connect(leaderBoardPage, &LeaderBoardWidget::backRequested, this, showMenu);

    connect(searchPlayerPage, &SearchPlayerWidget::playerSelected, this, [=](Player* player)
    {
        if (!player) return;
        playerDetailsPage->setPlayer(player);
        pages->setCurrentWidget(playerDetailsPage);
    });
    connect(searchTournamentPage, &SearchTournamentWidget::tournamentSelected, this, [=](Tournament* tournament)
    {
        if (!tournament) return;
        tournamentDetailsPage->setTournament(tournament);
        pages->setCurrentWidget(tournamentDetailsPage);
    });
    connect(playerDetailsPage, &PlayerDetailsWidget::backRequested, this, [=]
    {
        playerDetailsPage->setPlayer(nullptr);
        pages->setCurrentWidget(searchPlayerPage);
    });
    connect(tournamentDetailsPage, &TournamentDetailsWidget::backRequested, this, [=]
    {
        tournamentDetailsPage->setTournament(nullptr);
        pages->setCurrentWidget(searchTournamentPage);
    });

    // Pages must not retain pointers into a vector when records are added.
    connect(createPlayerPage, &CreatePlayerWidget::playersAboutToChange, this, [=]
    {
        playerDetailsPage->setPlayer(nullptr);
        searchPlayerPage->clearResults();
        tournamentDetailsPage->setTournament(nullptr);
        searchTournamentPage->clearResults();
    });
    connect(createTournamentPage, &CreateTournamentWidget::tournamentsAboutToChange, this, [=]
    {
        tournamentDetailsPage->setTournament(nullptr);
        searchTournamentPage->clearResults();
    });
    const auto showError = [this, pages, mainMenu](const QString& message)
    {
        pages->setCurrentWidget(mainMenu);
        QMessageBox::warning(this, "MatchForge", message);
    };
    connect(createPlayerPage, &CreatePlayerWidget::errorOccurred, this, showError);
    connect(createTournamentPage, &CreateTournamentWidget::errorOccurred, this, showError);
    connect(searchPlayerPage, &SearchPlayerWidget::errorOccurred, this, showError);
    connect(searchTournamentPage, &SearchTournamentWidget::errorOccurred, this, showError);
    connect(playerDetailsPage, &PlayerDetailsWidget::errorOccurred, this, showError);
    connect(tournamentDetailsPage, &TournamentDetailsWidget::errorOccurred, this, showError);
    connect(leaderBoardPage, &LeaderBoardWidget::errorOccurred, this, showError);
}
