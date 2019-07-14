#ifndef ABSTRACT_VARIABLE_H
#define ABSTRACT_VARIABLE_H

#include <QWidget>
#include <variable.h>
#include <QGridLayout>
#include <QComboBox>
#include <list>

class abstract_variable: public QWidget
{
    Q_OBJECT

    typedef std::list<variable*> vars_type;
    typedef std::map<int,QString> valmap_type;
    typedef valmap_type::const_iterator valmap_iter;

    variable* _variable;
    QComboBox* _combo;
    static vars_type* _checked;

private slots:
    void set_checked (int arg);
    void setIndexZero(int arg);

public:
    abstract_variable(variable* var, QGridLayout* layout, int row, QWidget* parent = 0);
    ~abstract_variable();
    static void set_statics(vars_type* arg);

};

#endif // ABSTRACT_VARIABLE_H
