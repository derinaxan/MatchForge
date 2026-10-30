#pragma once

#include <QWidget>
#include "MainManager.h"
#include <QString>
#include <vector>

#include "Tournament.h"

class QLineEdit;
class QListWidget;
class QPushButton;
class QLabel;

class SearchTournamentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchTournamentWidget(MainManager& manager, QWidget* parent = nullptr);

    void setResults(const std::vector<Tournament*>& tournaments);
    void clearResults();

signals:
    void errorOccurred(const QString& message);
    void backRequested();

    void tournamentSelected(Tournament* tournament);

private:
    MainManager& manager;
    QLineEdit* nameInput;
    QListWidget* resultsList;
    QPushButton* openButton;
    QLabel* statusLabel;

    std::vector<Tournament*> results;

    void submitSearch();
    void openSelectedTournament();
};