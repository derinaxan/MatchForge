#include <exception>
#include "SearchPlayerWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

SearchPlayerWidget::SearchPlayerWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(16);

    layout->addWidget(new QLabel("Search Player", this));

    nameInput = new QLineEdit(this);
    surnameInput = new QLineEdit(this);

    auto* form = new QFormLayout();
    form->addRow("enter player name", nameInput);
    form->addRow("enter player surname", surnameInput);

    layout->addLayout(form);

    auto* searchButton = new QPushButton("Search Player", this);
    layout->addWidget(searchButton);

    statusLabel = new QLabel("Enter player name and surname.", this);
    layout->addWidget(statusLabel);

    resultsList = new QListWidget(this);
    layout->addWidget(resultsList, 1);

    auto* backButton = new QPushButton("Back", this);

    openButton = new QPushButton("view player stats", this);
    openButton->setEnabled(false);

    auto* buttons = new QHBoxLayout();
    buttons->addWidget(backButton);
    buttons->addStretch();
    buttons->addWidget(openButton);

    layout->addLayout(buttons);

    connect(backButton, &QPushButton::clicked,
            this, &SearchPlayerWidget::backRequested);

    connect(searchButton, &QPushButton::clicked,
            this, &SearchPlayerWidget::submitSearch);

    connect(openButton, &QPushButton::clicked,
            this, &SearchPlayerWidget::openSelectedPlayer);

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
        openSelectedPlayer();
    });

    connect(nameInput, &QLineEdit::textChanged,
            this, &SearchPlayerWidget::clearResults);

    connect(surnameInput, &QLineEdit::textChanged,
            this, &SearchPlayerWidget::clearResults);
}

void SearchPlayerWidget::clearResults()
{
    results.clear();
    resultsList->clear();
    openButton->setEnabled(false);

    statusLabel->setText("Enter player name and surname.");
}

void SearchPlayerWidget::submitSearch()
{
    const QString name = nameInput->text().trimmed();
    const QString surname = surnameInput->text().trimmed();

    if (name.isEmpty() || surname.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Search Player",
            "Please enter player name and surname."
        );
        return;
    }

    clearResults();

    try
    {
        std::string first = name.toStdString();
        std::string last = surname.toStdString();
        setResults(manager.searchPlayer(first, last));
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }
}

void SearchPlayerWidget::setResults(
    const std::vector<Player*>& players)
{
    clearResults();

    for (Player* player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        results.push_back(player);

        resultsList->addItem(
            QString("%1 %2 | strength: %3")
                .arg(QString::fromStdString(player->getName()))
                .arg(QString::fromStdString(player->getSurname()))
                .arg(player->getStrength())
        );
    }

    statusLabel->setText(
        results.empty()
            ? "No player found"
            : QString("%1 results").arg(static_cast<int>(results.size()))
    );
}

void SearchPlayerWidget::openSelectedPlayer()
{
    const int row = resultsList->currentRow();

    if (row < 0 || row >= static_cast<int>(results.size()))
    {
        return;
    }

    emit playerSelected(results[row]);
}