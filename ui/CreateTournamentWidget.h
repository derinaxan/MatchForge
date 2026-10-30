#pragma once 

#include <QWidget>
#include "MainManager.h"
#include <QString>

#include "SportsType.h"
#include "TournamentSize.h"

class QLineEdit;
class QSpinBox;
class QComboBox;

class CreateTournamentWidget: public QWidget
{
    Q_OBJECT 
public:
    explicit CreateTournamentWidget(MainManager& manager, QWidget* parent = nullptr);
    void clearForm();
signals:
    void errorOccurred(const QString& message);
    void backRequested();
    void tournamentsAboutToChange();
    void tournamentCreated();

private:
    MainManager& manager;
    QLineEdit* nameInput;
    QSpinBox* yearInput;
    QComboBox* typeInput;
    QComboBox* sizeInput; 
    void submitTournament();


};