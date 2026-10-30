#pragma once
#include <QMainWindow>
#include "MainManager.h"

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    MainManager manager;
};
