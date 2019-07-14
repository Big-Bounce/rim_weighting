#include <abstract_axis.h>
#include <font_title.h>
#include <QSizePolicy>
#include <algorithm>
#include <abstract_cell.h>
#include <QVBoxLayout>
#include <cassert>
#include <utility>
#include <QHBoxLayout>

QStackedWidget* abstract_axis::_stack = nullptr;
abstract_axis** abstract_axis::_top = nullptr;
std::list<variable*>* abstract_axis::_checked = nullptr;

abstract_axis::abstract_axis(QString const& title, QAction* action, tree* arg_tree, QWidget* parent): QFrame(parent),
    _tree(arg_tree),
    _action(action),
    _scroll(new QScrollArea)
{
    QLabel* label = new QLabel(title);
    label -> setFont(font_title());
    label -> setFrameShape(QFrame::NoFrame);
    label -> setFrameShadow(QFrame::Sunken);

    QSizePolicy policy = label -> sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    label -> setSizePolicy(policy);

    QPushButton* button = new QPushButton(QString("Clear"));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(clear(bool)));

    QHBoxLayout* header_layout = new QHBoxLayout;
    header_layout -> setAlignment(Qt::AlignLeft);
    header_layout -> addWidget(label);
    header_layout -> addWidget(button);
    header_layout -> setAlignment(button, Qt::AlignRight);

    QFrame* header = new QFrame;
    header -> setLayout(header_layout);

    _scroll -> setWidgetResizable(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout -> setAlignment(Qt::AlignTop);
    layout -> addWidget(header);
    layout -> addWidget(_scroll);
    setLayout(layout);

    connect(_action, SIGNAL(triggered()), this, SLOT(show_me()));
    connect(_stack, SIGNAL(widgetRemoved(int)), this, SLOT(amibeeingdestroyed(int)));
    connect(_stack, SIGNAL(currentChanged(int)), this, SLOT(amibeeingontop(int)));
}

void abstract_axis::set_statics(QStackedWidget* stack, abstract_axis** top, vars_type* checked)
{
    _stack = stack;
    _top = top;
    _checked = checked;
}

void abstract_axis::show_me()
{
    _stack -> setCurrentWidget(this);
}

void abstract_axis::amibeeingdestroyed(int arg)
{
    if (_stack -> indexOf(this) < 0)
        delete _action;
}

void abstract_axis::amibeeingontop(int arg)
{
    if (_stack -> indexOf(this) == arg)
    {
        *_top = this;
        emit changed(!_tree -> is_skewed());
    }
}

abstract_axis::~abstract_axis()
{
}

void abstract_axis::add()
{
    std::vector<double> values(_cells.size());
    values.clear();
    for (cellsmap_iter iter = _cells.cbegin(); iter != _cells.cend(); ++iter)
         values.push_back(iter -> second -> value());

    for (vars_iter iter = _checked->begin(); iter != _checked->end(); ++iter)
        _tree -> add_level(*iter);
    renew_view(std::move(std::vector<double> ()));
}

void abstract_axis::remove()
{
    for (vars_iter iter = _checked->begin(); iter != _checked->end(); ++iter)
        _tree -> erase_level(*iter);
    renew_view(std::move(std::vector<double> ()));
}

void abstract_axis::add_to_cell(tree::node* arg)
{
    cells_type previous(_tree->cells());
    for (vars_iter iter = _checked->begin(); iter != _checked->end(); ++iter)
        _tree -> add_level(*iter, arg);
    update(std::move(previous));
}

void abstract_axis::remove_from_cell(tree::node* arg)
{
    cells_type previous(_tree->cells());
    tree::node* temp = _tree -> find_subtree_root(arg);
    for (vars_iter iter = _checked->begin(); iter != _checked->end(); ++iter)
        _tree -> erase_level(*iter, temp);
    update(std::move(previous));
}

const tree* abstract_axis::my_tree() const
{
    return _tree;
}

void abstract_axis::update(cells_type &&previous)
{
    for (cells_iter iter = previous.cbegin(); iter != previous.cend(); ++iter)
        if (std::find(_tree->cells().cbegin(), _tree->cells().cend(), *iter) == _tree->cells().cend())
        {
            assert(_cells.find(*iter) != _cells.end());
            delete _cells[*iter];
            _cells.erase(*iter);
        }

    assert(_scroll -> widget());
    assert(_scroll -> widget() -> layout());

    emit changed(!_tree -> is_skewed());

    for (cells_iter iter = _tree->cells().cbegin(); iter != _tree->cells().cend(); ++iter)
        if (std::find(previous.cbegin(), previous.cend(), *iter) == previous.cend())
        {
            abstract_cell* cell = new abstract_cell(*iter, this);
            _scroll -> widget() -> layout() -> addWidget(cell);
            _cells[*iter] = cell;
            connect(this, SIGNAL(changed(bool)), cell, SLOT(check_moderability(bool)));
        }
        else
        {
            assert(_cells.find(*iter) != _cells.end());
            _cells[*iter] -> update();
        }
}

void abstract_axis::renew_view(std::vector<double> &&values)
{

    QVBoxLayout* layout = new QVBoxLayout;
    layout -> setAlignment(Qt::AlignTop);
    _cells.clear();

    size_t i = 0;
    for (cells_iter iter = _tree->cells().cbegin(); iter != _tree -> cells().cend(); ++iter)
    {
        abstract_cell* cell = new abstract_cell(*iter, this, (values.empty() ? 0.0 : (i++ == values.size() ? values[i = 0] : values[i - 1])));
        layout -> addWidget(cell);
        _cells[*iter] = cell;
        connect(this, SIGNAL(changed(bool)), cell, SLOT(check_moderability(bool)));
    }


     if (_scroll -> widget())
        delete _scroll -> widget();
    QFrame* frame = new QFrame;
    frame -> setLayout(layout);
    _scroll -> setWidget(frame);

}

void abstract_axis::clear(bool)
{
    _tree -> clear();
    renew_view(std::move(std::vector<double> ()));
    emit changed(!_tree -> is_skewed());
}

std::map<tree::node*, abstract_cell*> const& abstract_axis::my_cells() const
{
    return _cells;
}
