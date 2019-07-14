#ifndef CUSTOM_LABEL_ELEMENT_H
#define CUSTOM_LABEL_ELEMENT_H

#include <QFrame>

class custom_label_element: public QFrame
{
    Q_OBJECT

public:
    custom_label_element(QString const& varname, QString const& value, QWidget* parent = 0);
    ~custom_label_element();

};


#endif // CUSTOM_LABEL_ELEMENT_H
