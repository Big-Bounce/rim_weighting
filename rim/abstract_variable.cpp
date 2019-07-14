#include <abstract_variable.h>
#include <QSizePolicy>
#include <QLabel>
#include <QCheckBox>
#include <custom_label.h>
#include <QScrollArea>

std::list <variable*>* abstract_variable::_checked = nullptr;

abstract_variable::abstract_variable(variable* var, QGridLayout* layout, int row, QWidget* parent): QWidget(parent),
    _variable(var),
    _combo(new QComboBox)
{

    QSizePolicy policy;

    //variable name
    custom_label* name = new custom_label(10, var -> name());
    policy = name -> sizePolicy();
    policy.setHorizontalStretch(1);
    name -> setSizePolicy(policy);
    layout -> addWidget(name, row, 0);
    layout -> setAlignment(name, Qt::AlignTop);

    //variable label
    custom_label* label = new custom_label(40, var -> label());
    policy = label -> sizePolicy();
    policy.setHorizontalStretch(4);
    label -> setSizePolicy(policy);
    layout -> addWidget(label, row, 1);
    layout -> setAlignment(label, Qt::AlignTop);

    //combo
    _combo -> addItem(QString(tr("[Values' list]")));
    for (valmap_iter iter = (var -> values()).begin(); iter != (var -> values()).end(); ++iter)
        _combo -> addItem(iter -> second);
    policy = _combo -> sizePolicy();
    _combo -> setMaximumWidth(120);
    _combo -> setSizePolicy(policy);
    layout -> addWidget(_combo, row, 2);
    layout -> setAlignment(_combo, Qt::AlignTop);
    connect(_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(setIndexZero(int)));

    //check
    QCheckBox* check = new QCheckBox;
    check -> setEnabled(var -> usability());
    layout -> addWidget(check, row, 3);
    layout -> setAlignment(check, Qt::AlignTop);
    connect(check, SIGNAL(stateChanged(int)), this, SLOT(set_checked(int)));

}

abstract_variable::~abstract_variable()
{

}

void abstract_variable::set_checked (int arg)
{
    if (arg)
        _checked -> push_back(_variable);
    else
        _checked -> remove(_variable);
}

void abstract_variable::setIndexZero(int arg)
{
    _combo -> setCurrentIndex(0);
}

void abstract_variable::set_statics(vars_type* arg)
{
    _checked = arg;
}
