#include <exception>
#include "LeaderBoardWidget.h"

#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

LeaderBoardWidget::LeaderBoardWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(16);

    layout->addWidget(new QLabel("View Leaderboard", this));

    typeInput = new QComboBox(this);

    typeInput->addItem(
        "FOOTBALL",
        static_cast<int>(SportsType::FOOTBALL)
    );

    typeInput->addItem(
        "BASKETBALL",
        static_cast<int>(SportsType::BASKETBALL)
    );

    layout->addWidget(typeInput);

    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(4);

    resultsTable->setHorizontalHeaderLabels(
        {"Rank", "Name", "Surname", "Points"}
    );

    resultsTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    resultsTable->verticalHeader()->hide();
    resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(resultsTable, 1);

    auto* backButton = new QPushButton("Back", this);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked,
            this, &LeaderBoardWidget::backRequested);

    connect(typeInput, &QComboBox::currentIndexChanged,
            this, [this](int)
    {
        resultsTable->setRowCount(0);
        refresh();
    });
}

SportsType LeaderBoardWidget::selectedType() const
{
    return static_cast<SportsType>(
        typeInput->currentData().toInt()
    );
}

void LeaderBoardWidget::setPlayers(const std::vector<Player>& players)
{
    resultsTable->setRowCount(static_cast<int>(players.size()));

    for (int row = 0; row < static_cast<int>(players.size()); ++row)
    {
        const Player& player = players[row];

        resultsTable->setItem(
            row, 0,
            new QTableWidgetItem(QString::number(row + 1))
        );

        resultsTable->setItem(
            row, 1,
            new QTableWidgetItem(
                QString::fromStdString(player.getName())
            )
        );

        resultsTable->setItem(
            row, 2,
            new QTableWidgetItem(
                QString::fromStdString(player.getSurname())
            )
        );

        resultsTable->setItem(
            row, 3,
            new QTableWidgetItem(
                QString::number(player.getPoints(selectedType()))
            )
        );
    }
}
bool LeaderBoardWidget::refresh()
{
    try
    {
        setPlayers(manager.getLeaderBoard(selectedType()));
        return true;
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }
    return false;
}
