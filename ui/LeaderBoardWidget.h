#pragma once

#include <QWidget>
#include "MainManager.h"
#include <vector>

#include "Player.h"
#include "SportsType.h"

class QComboBox;
class QTableWidget;

class LeaderBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaderBoardWidget(MainManager& manager, QWidget* parent = nullptr);

    bool refresh();
    SportsType selectedType() const;
    void setPlayers(const std::vector<Player>& players);

signals:
    void errorOccurred(const QString& message);
    void backRequested();


private:
    MainManager& manager;
    QComboBox* typeInput;
    QTableWidget* resultsTable;
};