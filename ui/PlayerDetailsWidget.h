#pragma once

#include <QWidget>
#include "MainManager.h"

#include "Player.h"

class QPlainTextEdit;

class PlayerDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerDetailsWidget(MainManager& manager, QWidget* parent = nullptr);
    void setPlayer(Player* player);

signals:
    void errorOccurred(const QString& message);
    void backRequested();


private:
    MainManager& manager;
    Player* selectedPlayer = nullptr;
    QPlainTextEdit* details;

    void refreshDetails();
    void changeTournament(bool add);
};