#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <custom_menubar.h>
#include <QStackedWidget>
#include <QDockWidget>
#include <QScrollArea>
#include <dataset.h>
#include <list>
#include <abstract_axis.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    custom_menubar* _menubar;
    QStackedWidget* _axes;
    QDockWidget* _variablesW;
    QScrollArea* _variablesA;
    QDockWidget* _buttons;
    dataset _dataset;
    std::list <variable*> _checked_variables;
    abstract_axis* _top_axis;

private slots:
    void add_axis();
    void remove_axis();
    void add_variables();
    void remove_variables();
    void load();
    void quit();
    void go();
    void options();
    void outwrite();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void enabling(bool);

public slots:
    void skewness_checking(bool arg);
};

#endif // MAINWINDOW_H
