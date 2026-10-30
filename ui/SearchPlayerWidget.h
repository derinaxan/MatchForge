#pragma once

#include <QWidget>
#include "MainManager.h"
#include <QString>
#include <vector>

#include "Player.h"

class QLineEdit;
class QListWidget;
class QPushButton;
class QLabel;

class SearchPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPlayerWidget(MainManager& manager, QWidget* parent = nullptr);

    void setResults(const std::vector<Player*>& players);
    void clearResults();

signals:
    void errorOccurred(const QString& message);
    void backRequested();



    void playerSelected(Player* player);

private:
    MainManager& manager;
    QLineEdit* nameInput;
    QLineEdit* surnameInput;
    QListWidget* resultsList;
    QPushButton* openButton;
    QLabel* statusLabel;

    std::vector<Player*> results;

    void submitSearch();
    void openSelectedPlayer();
};