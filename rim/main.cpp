#include <mainwindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    try
    {
        QApplication app(argc, argv);
        MainWindow rim;
        rim.showMaximized();
        return app.exec();
    }
    catch (...)
    {
        return 1;
    }


}
