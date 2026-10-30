#pragma once

#include <QWidget>
#include "MainManager.h"

#include "Tournament.h"

class QPlainTextEdit;
class QPushButton;

class TournamentDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TournamentDetailsWidget(MainManager& manager, QWidget* parent = nullptr);
    void setTournament(Tournament* tournament);

signals:
    void errorOccurred(const QString& message);
    void backRequested();


private:
    MainManager& manager;
    Tournament* selectedTournament = nullptr;

    QPlainTextEdit* details;
    QPushButton* startButton;
    QPushButton* simulateButton;

    void refreshDetails();
};