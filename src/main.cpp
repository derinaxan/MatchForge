#include <QApplication>
#include "../ui/MainWindow.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("MatchForge");
    window.resize(1100, 700);
    window.show();

    return app.exec();
}

//cmake -S. -B build
//cmake --build build
//.\build\intro.exe 