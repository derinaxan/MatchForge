#include <exception>
#include "SearchTournamentWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

SearchTournamentWidget::SearchTournamentWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(16);

    layout->addWidget(new QLabel("Search tournament", this));

    nameInput = new QLineEdit(this);

    auto* form = new QFormLayout();
    form->addRow("enter tournament name", nameInput);

    layout->addLayout(form);

    auto* searchButton = new QPushButton("Search tournament", this);
    layout->addWidget(searchButton);

    statusLabel = new QLabel("Enter tournament name.", this);
    layout->addWidget(statusLabel);

    resultsList = new QListWidget(this);
    layout->addWidget(resultsList, 1);

    auto* backButton = new QPushButton("Back", this);

    openButton = new QPushButton("look at detailed stats", this);
    openButton->setEnabled(false);

    auto* buttons = new QHBoxLayout();
    buttons->addWidget(backButton);
    buttons->addStretch();
    buttons->addWidget(openButton);

    layout->addLayout(buttons);

    connect(backButton, &QPushButton::clicked,
            this, &SearchTournamentWidget::backRequested);

    connect(searchButton, &QPushButton::clicked,
            this, &SearchTournamentWidget::submitSearch);

    connect(openButton, &QPushButton::clicked,
            this, &SearchTournamentWidget::openSelectedTournament);

    connect(resultsList, &QListWidget::currentRowChanged,
            this, [this](int row)
    {
        openButton->setEnabled(
            row >= 0 && row < static_cast<int>(results.size())
        );
    });

    connect(resultsList, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem*)
    {
        openSelectedTournament();
    });

    connect(nameInput, &QLineEdit::textChanged,
            this, &SearchTournamentWidget::clearResults);
}

void SearchTournamentWidget::clearResults()
{
    results.clear();
    resultsList->clear();
    openButton->setEnabled(false);

    statusLabel->setText("Enter tournament name.");
}

void SearchTournamentWidget::submitSearch()
{
    const QString name = nameInput->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Search tournament",
            "Please enter tournament name."
        );
        return;
    }

    clearResults();

    try
    {
        std::string tournamentName = name.toStdString();
        setResults(manager.searchTournament(tournamentName));
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }
}

void SearchTournamentWidget::setResults(
    const std::vector<Tournament*>& tournaments)
{
    clearResults();

    for (Tournament* tournament : tournaments)
    {
        if (tournament == nullptr)
        {
            continue;
        }

        results.push_back(tournament);

        resultsList->addItem(
            QString("%1 (%2) | size: %3")
                .arg(QString::fromStdString(tournament->getName()))
                .arg(tournament->getYear())
                .arg(static_cast<int>(tournament->getSize()))
        );
    }

    statusLabel->setText(
        results.empty()
            ? "No tournament found"
            : QString("%1 results").arg(static_cast<int>(results.size()))
    );
}

void SearchTournamentWidget::openSelectedTournament()
{
    const int row = resultsList->currentRow();

    if (row < 0 || row >= static_cast<int>(results.size()))
    {
        return;
    }

    emit tournamentSelected(results[row]);
}