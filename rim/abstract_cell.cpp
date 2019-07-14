#include <abstract_cell.h>
#include <custom_label_element.h>
#include <cassert>

abstract_cell::abstract_cell(tree::node* cell, abstract_axis* axis, double value, QWidget* parent): QScrollArea(parent),
    _cell(cell),
    _axis(axis),
    _spin(new QDoubleSpinBox),
    _add(new QPushButton(tr("Add"))),
    _remove(new QPushButton(tr("Remove")))
{

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
    setMinimumHeight(100);
    setWidget(make_cell(value));

    // ----- connecting ----------
    connect(_add, SIGNAL(clicked(bool)), this, SLOT(add_to_me(bool)));
    connect(this, SIGNAL(my_cell_add(tree::node*)), _axis, SLOT(add_to_cell(tree::node*)));
    connect(_remove, SIGNAL(clicked(bool)), this, SLOT(remove_from_me(bool)));
    connect(this, SIGNAL(my_cell_remove(tree::node*)), _axis, SLOT(remove_from_cell(tree::node*)));

}

abstract_cell::~abstract_cell()
{
}

void abstract_cell::update()
{
    assert(widget());
    delete layout();
    setWidget(make_cell());
}

void abstract_cell::add_to_me(bool arg)
{
    emit my_cell_add(_cell);
}

void abstract_cell::remove_from_me(bool arg)
{
    emit my_cell_remove(_cell);
}

void abstract_cell::check_moderability(bool arg)
{
    _remove -> setEnabled(_cell -> is_moderable() || arg);
}

QFrame* abstract_cell::make_cell(double value)
{

    QHBoxLayout* layout = new QHBoxLayout;
    layout -> setAlignment(Qt::AlignLeft);

    // ----- variables and values ----------

    const tree::node* temp = _cell;
    while (temp -> my_parent() != 0x0)
    {
        layout -> addWidget
                (new custom_label_element(temp->my_parent()->my_next()->name(),temp->my_parent()->my_next()->values().at(temp -> my_value())));
        temp = temp -> my_parent();
    }

    // ----- preparing view ----------

    QFrame* vars_frame = new QFrame(this);
    vars_frame -> setLayout(layout);

    _spin -> setValue(value);
    _spin -> setMinimum(0);
    _spin -> setDecimals(5);
    _spin -> setSingleStep(1);

    QHBoxLayout* spin_layout = new QHBoxLayout;
    spin_layout -> setAlignment(Qt::AlignRight);
    spin_layout -> addWidget(_spin);
    spin_layout -> addWidget(_add);
    spin_layout -> addWidget(_remove);

    QFrame* spin_frame = new QFrame(this);
    spin_frame -> setLayout(spin_layout);

    QHBoxLayout* external = new QHBoxLayout;
    external -> addWidget(vars_frame);
    external -> addWidget(spin_frame);

    QFrame* all = new QFrame;
    QSizePolicy policy(all -> sizePolicy());
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    all -> setSizePolicy(policy);
    all -> setLayout(external);
    all -> setFrameShape(QFrame::NoFrame);

    return all;
}

volumes abstract_cell::make_volumes()
{
    return volumes(_spin -> value());
}

tree::node* abstract_cell::my_cell() const
{
    return _cell;
}
