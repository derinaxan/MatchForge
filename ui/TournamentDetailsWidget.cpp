#include <exception>
#include "TournamentDetailsWidget.h"

#include <QLabel>
#include <algorithm>
#include <stdexcept>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
QString roundName(Round round)
{
    switch (round)
    {
        case Round::LAST_32:        return "Last 32";
        case Round::LAST_16:        return "Last 16";
        case Round::QUARTER_FINALS: return "Quarter finals";
        case Round::SEMI_FINALS:    return "Semi finals";
        case Round::FINAL:          return "Final";
        case Round::FINISHED:       return "Finished";
    }

    return "Unknown";
}

QString playerName(const Player& player)
{
    return QString::fromStdString(player.getName()) + " " +
           QString::fromStdString(player.getSurname());
}
}

TournamentDetailsWidget::TournamentDetailsWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(16);

    layout->addWidget(new QLabel("look at detailed stats", this));

    details = new QPlainTextEdit(this);
    details->setReadOnly(true);
    layout->addWidget(details, 1);

    auto* statsButton = new QPushButton("look at detailed stats", this);
    startButton = new QPushButton("Start tournament", this);
    simulateButton = new QPushButton("Simulate current Round", this);
    auto* backButton = new QPushButton("Back", this);

    startButton->setEnabled(false);
    simulateButton->setEnabled(false);

    layout->addWidget(statsButton);
    layout->addWidget(startButton);
    layout->addWidget(simulateButton);
    layout->addWidget(backButton);

    connect(statsButton, &QPushButton::clicked,
            this, &TournamentDetailsWidget::refreshDetails);

    connect(backButton, &QPushButton::clicked,
            this, &TournamentDetailsWidget::backRequested);

    connect(startButton, &QPushButton::clicked,
            this, [this]()
    {
        if (selectedTournament != nullptr)
        {
            try
            {
                if (selectedTournament->getHasStarted()) return;
                if (selectedTournament->getPlayers().size() != static_cast<std::size_t>(selectedTournament->getSize()))
                    throw std::runtime_error("Fill all tournament places before starting.");
                manager.startTournament(*selectedTournament);
                refreshDetails();
                manager.save();
            }
            catch (const std::exception& error)
            {
                emit errorOccurred(QString::fromUtf8(error.what()));
            }
        }
    });

    connect(simulateButton, &QPushButton::clicked,
            this, [this]()
    {
        if (selectedTournament != nullptr)
        {
            try
            {
                if (!selectedTournament->getHasStarted() || selectedTournament->getCurrentRound() == Round::FINISHED) return;
                const auto& matches = selectedTournament->getMatches();
                const auto currentRound = selectedTournament->getCurrentRound();
                const auto count = std::count_if(matches.begin(), matches.end(), [currentRound](const Match& match)
                { return match.getRound() == currentRound; });
                if (count != static_cast<int>(currentRound) / 2)
                    throw std::runtime_error("The current round has missing matches.");
                manager.simulateRound(*selectedTournament);
                refreshDetails();
                manager.save();
            }
            catch (const std::exception& error)
            {
                emit errorOccurred(QString::fromUtf8(error.what()));
            }
        }
    });
}

void TournamentDetailsWidget::setTournament(Tournament* tournament)
{
    selectedTournament = tournament;
    refreshDetails();
}

void TournamentDetailsWidget::refreshDetails()
{
    if (selectedTournament == nullptr)
    {
        details->clear();
        startButton->setEnabled(false);
        simulateButton->setEnabled(false);
        return;
    }

    Tournament& tournament = *selectedTournament;

    QString text = "tournament name: ";
    text += QString::fromStdString(tournament.getName());

    text += "\nyear: " + QString::number(tournament.getYear());
    text += "\nsize: ";
    text += QString::number(static_cast<int>(tournament.getSize()));

    text += "\ncurrent tournament round: ";
    text += roundName(tournament.getCurrentRound());

    text += "\n\nplayers:\n";

    for (const Player* player : tournament.getPlayers())
    {
        if (player != nullptr)
        {
            text += playerName(*player) + "\n";
        }
    }

    text += "\nmatches:\n";

    for (const Match& match : tournament.getMatches())
    {
        text += roundName(match.getRound()) + " | ";
        text += playerName(match.getHomePlayer()) + " - ";
        text += playerName(match.getAwayPlayer());

        // Mevcut modelde tamamlanan turların enum değeri daha büyük.
        const bool played =
            static_cast<int>(match.getRound()) >
            static_cast<int>(tournament.getCurrentRound());

        if (played)
        {
            const Score score = match.getScore();

            text += QString(" | %1 : %2")
                .arg(score.homeScore)
                .arg(score.awayScore);
        }
        else
        {
            text += " | Not played";
        }

        text += "\n";
    }

    if (tournament.getWinner() != nullptr)
    {
        text += "\nwinner: " + playerName(*tournament.getWinner());
    }

    details->setPlainText(text);

    const bool full =
        tournament.getPlayers().size() ==
        static_cast<std::size_t>(tournament.getSize());

    startButton->setEnabled(
        !tournament.getHasStarted() && full
    );

    simulateButton->setEnabled(
        tournament.getHasStarted() &&
        tournament.getCurrentRound() != Round::FINISHED
    );
}



