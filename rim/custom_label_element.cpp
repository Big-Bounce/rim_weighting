#include <custom_label_element.h>
#include <QFont>
#include <QVBoxLayout>
#include <QLabel>

custom_label_element::custom_label_element(QString const& varname, QString const& value, QWidget* parent): QFrame(parent)
{
    QLabel* top = new QLabel(varname);
    QLabel* bottom = new QLabel(value);

    QFont font(top -> font());
    font.setBold(true);
    top -> setFont(font);
    top -> setWordWrap(true);
    top -> setAlignment(Qt::AlignLeft);
    bottom -> setWordWrap(true);
    bottom -> setAlignment(Qt::AlignLeft);

    QVBoxLayout* layout = new QVBoxLayout;
    layout -> addWidget(top);
    layout -> addWidget(bottom);

    setFixedWidth(120);
    setLayout(layout);
}

custom_label_element::~custom_label_element()
{
}
