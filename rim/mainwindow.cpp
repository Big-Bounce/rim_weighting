#include <mainwindow.h>
#include <QString>
#include <QInputDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <abstract_variable.h>
#include <QApplication>
#include <QFileDialog>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <climits>
#include <abstract_cell.h>
#include <cfloat>
#include <QComboBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    _menubar(new custom_menubar),
    _axes(new QStackedWidget),
    _variablesW(new QDockWidget(this)),
    _variablesA(new QScrollArea),
    _buttons(new QDockWidget(this)),
    _top_axis(nullptr)
{
    //============ menu bar =================
    _menubar -> menu(QString("Program")) -> addAction(QString(tr("Load")), this, load);
    _menubar -> menu(QString("Program")) -> addAction(QString(tr("Quit")), this, quit);
    _menubar -> menu(QString("Manage")) -> addAction(QString(tr("Add axis")), this, add_axis);
    _menubar -> menu(QString("Manage")) -> addAction(QString(tr("Remove axis")), this, remove_axis);
    _menubar -> menu(QString("Manage")) -> addAction(QString(tr("Add variables")), this, add_variables);
    connect (this, SIGNAL(enabling(bool)),
    _menubar -> menu(QString("Manage")) -> addAction(QString(tr("Remove variables")), this, remove_variables), SLOT(setEnabled(bool)));
    _menubar -> menu(QString("Weighting")) -> addAction(QString(tr("Options")), this, options);
    _menubar -> menu(QString("Weighting")) -> addAction(QString(tr("Go!")), this, go);
    _menubar -> menu(QString("Weighting")) -> addAction(QString(tr("Outwrite")), this, outwrite);
    setMenuBar(_menubar);

    //============ axes view =================
    abstract_axis::set_statics(_axes, &_top_axis, &_checked_variables);
    setCentralWidget(_axes);

    //============ variables view =============
    abstract_variable::set_statics(&_checked_variables);
    _variablesA -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _variablesA -> setWidgetResizable(true);
    _variablesW -> setFeatures(0);
    _variablesW -> setMinimumWidth(600);
    _variablesW -> setWidget(_variablesA);
    addDockWidget(Qt::LeftDockWidgetArea, _variablesW);

     //============ buttons =================

    QPushButton* add_axis_button = new QPushButton(tr("Add axis"));
    QPushButton* remove_axis_button = new QPushButton(tr("Remove axis"));
    QPushButton* add_variable_button = new QPushButton(tr("Add variables"));
    QPushButton* remove_variable_button = new QPushButton(tr("Remove variables"));

    QPushButton* go_button = new QPushButton(tr("Go!"));
    QSizePolicy policy(go_button -> sizePolicy());
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    go_button -> setSizePolicy(policy);

    connect(this, SIGNAL(enabling(bool)), remove_variable_button, SLOT(setEnabled(bool)));
    connect(add_axis_button, SIGNAL(clicked()), this, SLOT(add_axis()));
    connect(remove_axis_button, SIGNAL(clicked()), this, SLOT(remove_axis()));
    connect(add_variable_button, SIGNAL(clicked()), this, SLOT(add_variables()));
    connect(remove_variable_button, SIGNAL(clicked()), this, SLOT(remove_variables()));
    connect(go_button, SIGNAL(clicked()), this, SLOT(go()));

    QGridLayout* buttons_layout = new QGridLayout;
    buttons_layout -> addWidget(add_axis_button,0,0);
    buttons_layout -> addWidget(remove_axis_button,0,1);
    buttons_layout -> addWidget(go_button,1,0,1,2);
    buttons_layout -> addWidget(add_variable_button,2,0);
    buttons_layout -> addWidget(remove_variable_button,2,1);

    QGroupBox* buttons = new QGroupBox(_buttons);
    buttons -> setLayout(buttons_layout);

    _buttons -> setFeatures(0);
    _buttons -> setMaximumHeight(150);
    _buttons -> setWidget(buttons);
    addDockWidget(Qt::LeftDockWidgetArea, _buttons);
}

MainWindow::~MainWindow()
{
}

void MainWindow::add_axis()
{
    bool ok;
    QString name = QInputDialog::getText(
                this, QString(tr("New axis' name")), QString(tr("Enter the name")), QLineEdit::Normal, QString(tr("New axis")), &ok);
    if (!ok)
        return;

    QAction* action =  _menubar -> menu(QString("Axes")) -> addAction(name);
    abstract_axis* temp = new abstract_axis(name, action, _dataset.add_tree());
    connect(temp, SIGNAL(changed(bool)), this, SLOT(skewness_checking(bool)));
    _axes -> addWidget(temp);
    _axes -> setCurrentWidget(temp);    
}

void MainWindow::remove_axis()
{
    if (_top_axis)
    {
        abstract_axis* temp = _top_axis;
        _axes -> removeWidget(_top_axis);
        _dataset.remove_tree(temp -> my_tree());
        delete temp;
    }

    if (!_axes -> count())
        _top_axis = nullptr;
}

void MainWindow::load()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Dictionary file"), "c:\\", "*.txt");
    if (path.isNull())
        return;

    //clearing
    while (_top_axis)
        remove_axis();
    if (_variablesA -> widget())
        delete _variablesA -> widget();
    _dataset.clear();
    _checked_variables.clear();

    //creating new ones
    try
    {
        _dataset.load_variables(path);
    }
    catch (bad_file_format_exc const & exc)
    {
        _dataset.clear();
        QMessageBox*  communicate = new QMessageBox(this);
        communicate -> setText(exc.whats());
        communicate -> addButton(QString("Ok"),QMessageBox::AcceptRole);
        communicate -> exec();
        return;
    }

    //data
    QString datapath = QFileDialog::getOpenFileName(this, tr("Data file"), "c:\\", "*.csv");
    if (datapath.isNull())
    {
        _dataset.clear();
        return;
    }
   try
   {
        _dataset.load_data(datapath);
   }
   catch (bad_file_format_exc const& exc)
   {
        _dataset.clear();
        QMessageBox*  communicate = new QMessageBox(this);
        communicate -> setText(exc.whats());
        communicate -> addButton(QString("Ok"),QMessageBox::AcceptRole);
        communicate -> exec();
        return;
   }



    QGridLayout* layout = new QGridLayout;
    for (std::vector<variable*>::size_type i = 0; i < _dataset.varlist().size(); ++i)
       new abstract_variable(_dataset.varlist().at(i), layout, i);

    QFrame* frame = new QFrame;
    frame -> setLayout(layout);
    _variablesA -> setWidget(frame);


}

void MainWindow::outwrite()
{
    QString datapath = QFileDialog::getSaveFileName(this, tr("Output file"), "c:\\", "*.csv");
    if (datapath.isNull())
        return;

    try {
        _dataset.outwrite(datapath, 0);
    }
    catch (no_weightings_done const & arg)
    {
        QMessageBox box;
        box.setText(arg.whats());
        box.addButton(QString("OK"), QMessageBox::AcceptRole);
        box.exec();
    }

}


void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::add_variables()
{
    if (_top_axis)
       _top_axis -> add();
}

void MainWindow::remove_variables()
{
    if (_top_axis)
       _top_axis -> remove();
}


void MainWindow::skewness_checking(bool arg)
{
    emit enabling(arg);
}

void MainWindow::go()
{
    typedef std::map<tree::node*, abstract_cell*>::const_iterator cells_iter;
    typedef std::map<tree::node*, volumes> rimtype;
    rimtype rim;

    for (int i = 0; i < _axes -> count(); ++i)
    {
        abstract_axis* axis = dynamic_cast<abstract_axis*> (_axes -> widget(i));
        rimtype temp;
        double sum(0.0);
        for (cells_iter iter = axis -> my_cells().cbegin(); iter != axis -> my_cells().cend(); ++iter)
        {
            temp[iter -> second -> my_cell()] = iter -> second -> make_volumes();
            sum += temp[iter -> second -> my_cell()].target;
        }
        for (rimtype::iterator iter = temp.begin(); iter != temp.end(); ++iter)
            rim[iter->first] = iter->second.target *= _dataset.total() / sum;
      }

    _dataset.weight(rim);
}

void MainWindow::options()
{

    QLabel* total = new QLabel(tr("Total: "));
    QSpinBox* input_total = new QSpinBox;
    input_total -> setMinimum(0);
    input_total -> setMaximum(INT_MAX);
    input_total -> setValue(_dataset.total());

    QLabel* iterations = new QLabel(tr("Iterations: "));
    QSpinBox* input_iterations = new QSpinBox;
    input_iterations -> setMinimum(1);
    input_iterations -> setMaximum(INT_MAX);
    input_iterations -> setValue(_dataset.iterations());

    QLabel* rms = new QLabel(tr("Rms: "));
    QDoubleSpinBox* input_rms = new QDoubleSpinBox;
    input_rms -> setMinimum(0.0);
    input_rms -> setMaximum(DBL_MAX);
    input_rms -> setValue(_dataset.rms());

    QLabel* id = new QLabel(tr("Id: "));
    QComboBox* input_id = new QComboBox;
    for (size_t i = 0; i <_dataset.varlist().size(); ++i)
        input_id -> addItem(_dataset.varlist()[i] -> name());
    input_id -> setCurrentIndex(_dataset.id());

    QPushButton* button = new QPushButton(tr("Set"));
    QGridLayout* layout = new QGridLayout;
    layout -> addWidget(total,0,0);
    layout -> addWidget(input_total,0,1);
    layout -> addWidget(iterations,1,0);
    layout -> addWidget(input_iterations,1,1);
    layout -> addWidget(rms,2,0);
    layout -> addWidget(input_rms,2,1);
    layout -> addWidget(id,3,0);
    layout -> addWidget(input_id,3,1);
    layout -> addWidget(button, 4, 0, 1, 2);

    QDialog* dialog = new QDialog(this);
    connect(button, SIGNAL(pressed()), dialog, SLOT(accept()));
    dialog -> setLayout(layout);
    if (dialog->exec())
    {
        _dataset.set_rms(input_rms -> value());
        _dataset.set_iterations(input_iterations -> value());
        _dataset.set_total(input_total -> value());
        _dataset.change_id(input_id -> currentIndex());
    }
}
