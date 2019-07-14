#include <custom_menubar.h>

custom_menubar::custom_menubar(QWidget* parent): QMenuBar(parent)
{
    _menus["Program"] = addMenu(tr("Program"));
    _menus["Manage"] = addMenu(tr("Manage"));
    _menus["Axes"] = addMenu(tr("Axes"));
    _menus["Weighting"] = addMenu(tr("Weighting"));
}

custom_menubar::~custom_menubar()
{
}

QMenu* custom_menubar::menu(const QString &arg)
{
    if (_menus.find(arg) != _menus.end())
        return _menus[arg];
    return 0;
}
