#include <exception>
#include "CreatePlayerWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

#include <limits>

CreatePlayerWidget::CreatePlayerWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(60,50,60,50);
    layout->setSpacing(20);

    auto* title = new QLabel("Create player", this);
    layout->addWidget(title);

    nameInput = new QLineEdit(this);
    surnameInput = new QLineEdit(this);
    strengthInput = new QSpinBox(this);

    strengthInput->setRange(
        std::numeric_limits<int>::lowest(),
        std::numeric_limits<int>::max()
    );
    
    auto* form = new QFormLayout();
    form->setSpacing(16);

    form->addRow("enter player name", nameInput);
    form->addRow("enter player surname", surnameInput);
    form->addRow("enter player strength", strengthInput);

    layout->addLayout(form);

    auto* backButton = new QPushButton("Back", this);
    auto* createButton =  new QPushButton("Create Player", this);
    
    auto* buttons = new QHBoxLayout();
    buttons->addWidget(backButton);
    buttons->addStretch();
    buttons->addWidget(createButton);

    layout->addLayout(buttons);
    layout->addStretch();
    connect(backButton, &QPushButton::clicked, this, &CreatePlayerWidget::backRequested);
    connect(createButton, &QPushButton::clicked, this, &CreatePlayerWidget::submitPlayer);



}

void CreatePlayerWidget::submitPlayer()
{
    const QString name = nameInput->text().trimmed();
    const QString surname = surnameInput->text().trimmed();
    const int strength = strengthInput->value();
    if(name.isEmpty())
    {
        QMessageBox::warning(this, "create player", "Please enter player name");
        nameInput->setFocus();
        return;
    }
    if(surname.isEmpty())
    {
        QMessageBox::warning(this, "create player", "Please enter player surname");
        surnameInput->setFocus();
        return;
    }
    try
    {
        std::string first = name.toStdString();
        std::string last = surname.toStdString();
        emit playersAboutToChange();
        manager.createPlayer(first, last, strength);
        manager.save();
        clearForm();
        QMessageBox::information(this, "Create player", "Player created.");
        emit playerCreated();
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }


}


void CreatePlayerWidget::clearForm()
{
    nameInput->clear();
    surnameInput->clear();
    strengthInput->setValue(0);
}