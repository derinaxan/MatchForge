#include <exception>
#include "CreateTournamentWidget.h"
#include "Tools.h"
#include <QDate>

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

CreateTournamentWidget::CreateTournamentWidget(MainManager& appManager, QWidget* parent)
    : QWidget(parent), manager(appManager)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(48, 40, 48, 40);
    layout->setSpacing(20);

    layout->addWidget(new QLabel("Create tournament", this));

    nameInput = new QLineEdit(this);

    yearInput = new QSpinBox(this);
    yearInput->setRange(1, 9999);

    typeInput = new QComboBox(this);
    typeInput->addItem("FOOTBALL");
    typeInput->addItem("BASKETBALL");

    sizeInput = new QComboBox(this);

    for (int size : {32, 16, 8, 4, 2})
    {
        sizeInput->addItem(QString::number(size), size);
    }

    auto* form = new QFormLayout();
    form->setSpacing(16);

    form->addRow("enter tournament name", nameInput);
    form->addRow("enter tournament year", yearInput);
    form->addRow("pick a sports type", typeInput);
    form->addRow("pick a size", sizeInput);

    layout->addLayout(form);

    auto* backButton = new QPushButton("Back", this);
    auto* createButton = new QPushButton("Create tournament", this);

    auto* buttons = new QHBoxLayout();
    buttons->addWidget(backButton);
    buttons->addStretch();
    buttons->addWidget(createButton);

    layout->addLayout(buttons);
    layout->addStretch();

    connect( backButton, &QPushButton::clicked, this, &CreateTournamentWidget::backRequested);
    connect(createButton, &QPushButton::clicked, this, &CreateTournamentWidget::submitTournament);

    clearForm();
}

void CreateTournamentWidget::clearForm()
{
    nameInput->clear();
    yearInput->setValue(QDate::currentDate().year());
    typeInput->setCurrentIndex(0);
    sizeInput->setCurrentIndex(0);
}

void CreateTournamentWidget::submitTournament()
{
    const QString name = nameInput->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Create tournament", "Please enter tournament name.");
        nameInput->setFocus();
        return;
    }
    
    const int year = yearInput->value();
    const SportsType type = Tools::sportsTypeStringToEnum(typeInput->currentText().toStdString());
    const TournamentSize size = Tools::sizeIntToEnum( sizeInput->currentData().toInt());

    try
    {
        std::string tournamentName = name.toStdString();
        emit tournamentsAboutToChange();
        manager.createTournament(tournamentName, year, type, size);
        manager.save();
        clearForm();
        QMessageBox::information(this, "Create tournament", "Tournament created.");
        emit tournamentCreated();
    }
    catch (const std::exception& error)
    {
        emit errorOccurred(QString::fromUtf8(error.what()));
    }
}