#include <exception>
#include "PlayerDetailsWidget.h"

#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <stdexcept>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

PlayerDetailsWidget::PlayerDetailsWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(16);

    layout->addWidget(new QLabel("view player stats", this));

    details = new QPlainTextEdit(this);
    details->setReadOnly(true);
    layout->addWidget(details, 1);

    auto* statsButton = new QPushButton("view player stats", this);

    auto* addButton = new QPushButton(
        "add Player to tournament", this
    );

    auto* removeButton = new QPushButton(
        "remove player from tournament", this
    );

    auto* backButton = new QPushButton("back", this);

    layout->addWidget(statsButton);
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(backButton);

    connect(statsButton, &QPushButton::clicked,
            this, &PlayerDetailsWidget::refreshDetails);

    connect(backButton, &QPushButton::clicked,
            this, &PlayerDetailsWidget::backRequested);

    connect(addButton, &QPushButton::clicked,
            this, [this]()
    {
        if (selectedPlayer != nullptr)
        {
            changeTournament(true);
        }
    });

    connect(removeButton, &QPushButton::clicked,
            this, [this]()
    {
        if (selectedPlayer != nullptr)
        {
            changeTournament(false);
        }
    });
}

void PlayerDetailsWidget::setPlayer(Player* player)
{
    selectedPlayer = player;
    refreshDetails();
}

void PlayerDetailsWidget::refreshDetails()
{
    if (selectedPlayer == nullptr)
    {
        details->clear();
        return;
    }

    QString text;

    text += "player name: ";
    text += QString::fromStdString(selectedPlayer->getName());

    text += "\nsurname: ";
    text += QString::fromStdString(selectedPlayer->getSurname());

    text += "\nstrength: ";
    text += QString::number(selectedPlayer->getStrength());

    text += "\n\nrankings:\n";

    for (const Ranking& ranking : selectedPlayer->getRankings())
    {
        const QString type =
            ranking.type == SportsType::FOOTBALL
                ? "FOOTBALL"
                : "BASKETBALL";

        text += type + ": " + QString::number(ranking.points) + "\n";
    }

    details->setPlainText(text);
}

void PlayerDetailsWidget::changeTournament(bool add)
{
    try
    {
        if (!selectedPlayer) return;
        const auto candidates = add ? manager.getNonPlayedTournaments(*selectedPlayer)
                                    : manager.getPlayedTournaments(*selectedPlayer);
        std::vector<Tournament*> available;
        QStringList labels;
        for (Tournament* tournament : candidates)
        {
            if (!tournament || tournament->getHasStarted()) continue;
            if (add && tournament->getPlayers().size() >= static_cast<std::size_t>(tournament->getSize())) continue;
            available.push_back(tournament);
            labels << QString("%1. %2 (%3)").arg(static_cast<int>(available.size()))
                .arg(QString::fromStdString(tournament->getName())).arg(tournament->getYear());
        }
        if (available.empty())
        {
            QMessageBox::information(this, "Tournament", "No available tournament.");
            return;
        }
        bool accepted = false;
        const QString choice = QInputDialog::getItem(this, "Tournament", "Select tournament",
                                                     labels, 0, false, &accepted);
        if (!accepted) return;
        const int index = labels.indexOf(choice);
        if (index < 0) return;
        bool changed = add ? manager.addPlayerToTournament(selectedPlayer, available[index])
                           : manager.removePlayerFromTournament(selectedPlayer, available[index]);
        if (!changed) throw std::runtime_error("Tournament membership could not be changed.");
        manager.save();
        refreshDetails();
        QMessageBox::information(this, "Tournament", add ? "Player added to tournament." : "Player removed from tournament.");
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }
}
