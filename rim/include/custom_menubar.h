#ifndef CUSTOM_MENUBAR_H
#define CUSTOM_MENUBAR_H

#include <QMenuBar>
#include <map>
#include <QString>
#include <QMenu>

class custom_menubar: public QMenuBar
{
    Q_OBJECT

    std::map<QString, QMenu*> _menus;

public:
    custom_menubar(QWidget* parent = 0);
    ~custom_menubar();

    QMenu* menu(QString const& arg);

};

#endif // CUSTOM_MENUBAR_H
