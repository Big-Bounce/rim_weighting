#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <QScrollArea>
#include <QLabel>

class custom_label: public QScrollArea
{
    Q_OBJECT

    int _hint;

public:
    custom_label(int hint, QString text, QWidget* parent = 0);
    ~custom_label();

    QSize sizeHint() const;
};


#endif // CUSTOM_LABEL_H
