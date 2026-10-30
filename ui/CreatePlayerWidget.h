#pragma once 

#include <QWidget>
#include "MainManager.h"
#include <QString> 

class QLineEdit; //includeluyor da encounter edilidihi yerde includeladan dah kisa compile time 
class QSpinBox;

class CreatePlayerWidget: public QWidget
{
    Q_OBJECT 

public:
    explicit CreatePlayerWidget(MainManager& manager, QWidget* parent = nullptr);
    void clearForm();

signals:
    void errorOccurred(const QString& message);
    void backRequested();
    void playersAboutToChange();
    void playerCreated();

private:
    MainManager& manager;
    QLineEdit* nameInput;
    QLineEdit* surnameInput;
    QSpinBox* strengthInput; 

    void submitPlayer();
};