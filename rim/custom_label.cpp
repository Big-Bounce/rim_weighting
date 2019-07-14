#include <custom_label.h>
#include <QSizePolicy>
#include <QLayout>

custom_label::custom_label(int hint, QString text, QWidget* parent): QScrollArea(parent), _hint(hint)
{
    QSizePolicy policy = sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    setMaximumHeight(20);
    setSizePolicy(policy);

    QLabel* label = new QLabel(text);
    label -> setAlignment(Qt::AlignLeft);
    setWidget(label);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
}

custom_label::~custom_label()
{

}

QSize custom_label::sizeHint() const
{
    QSize size_inst = size();
    size_inst.setWidth(_hint);
    return size_inst;
}
