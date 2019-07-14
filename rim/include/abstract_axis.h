#ifndef ABSTRACT_AXIS_H
#define ABSTRACT_AXIS_H

#include <QFrame>
#include <QAction>
#include <QString>
#include <QLabel>
#include <tree.h>
#include <QStackedWidget>
#include <QScrollArea>
#include <list>
#include <map>
#include <vector>

class abstract_cell;

class abstract_axis: public QFrame
{
    Q_OBJECT

    //aliases
    typedef std::list<tree::node*> cells_type;
    typedef cells_type::const_iterator cells_iter;
    typedef std::list<variable*> vars_type;
    typedef vars_type::const_iterator vars_iter;
    typedef std::map<tree::node*, abstract_cell*> cellsmap_type;
    typedef cellsmap_type::const_iterator cellsmap_iter;

    tree* _tree;
    QAction* _action;
    QScrollArea* _scroll;
    cellsmap_type _cells;

    static QStackedWidget* _stack;
    static abstract_axis** _top;
    static vars_type* _checked;

    void renew_view(std::vector<double> &&values);
    void update(cells_type &&previous);

private slots:
    void show_me();
    void amibeeingdestroyed (int arg);
    void amibeeingontop (int arg);
    void clear (bool);

public:
    abstract_axis(QString const& title, QAction* action, tree* arg_tree, QWidget* parent = 0);
    ~abstract_axis();

    static void set_statics(QStackedWidget* stack, abstract_axis** top, vars_type* checked);
    void add();
    void remove();
    const tree* my_tree() const;
    cellsmap_type const& my_cells() const;

public slots:
     void add_to_cell(tree::node* arg);
     void remove_from_cell(tree::node* arg);

signals:
    void changed(bool);

};

#endif // ABSTRACT_AXIS_H
