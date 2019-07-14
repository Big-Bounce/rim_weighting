#ifndef ABSTRACT_CELL_H
#define ABSTRACT_CELL_H

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <tree.h>
#include <QPushButton>
#include <abstract_axis.h>
#include <structures.h>

class abstract_cell: public QScrollArea
{
    Q_OBJECT

    tree::node* _cell;
    abstract_axis* _axis;
    QDoubleSpinBox* _spin;
    QPushButton* _add;
    QPushButton* _remove;

    QFrame* make_cell(double value = 0.0);

signals:
    void my_cell_add(tree::node*);
    void my_cell_remove(tree::node*);

private slots:
    void add_to_me(bool arg);
    void remove_from_me(bool arg);

public slots:
    void check_moderability(bool arg);

public:
    abstract_cell(tree::node* cell, abstract_axis* axis, double value = 0.0, QWidget* parent = 0);
    ~abstract_cell();

    void update();
    inline double value() const {return _spin->value();}
    volumes make_volumes();
    tree::node* my_cell() const;

};


#endif // ABSTRACT_CELL_H
