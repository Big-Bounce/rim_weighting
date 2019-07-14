#include <variable.h>

variable::variable(QString const& arg_name, QString const& arg_label):
    _name(arg_name),
    _label(arg_label),
    _usability(false)
{
}

variable::variable(QString const& arg_name, QString const& arg_label, std::map<int,QString> const& arg_values):
    _name(arg_name),
    _label(arg_label),
    _usability(true),
    _vallist(arg_values)
{
}
