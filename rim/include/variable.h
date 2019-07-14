#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include <QString>
#include <vector>
#include <map>

class variable
{

    QString _name;
    QString _label;
    bool _usability;
    std::map<int,QString> _vallist;
    std::vector<int> _data;

public:
    variable(){}
    variable(QString const& arg_name, QString const& arg_label);
    variable(QString const& arg_name, QString const& arg_label, std::map<int,QString> const& arg_values);
    ~variable(){}

    inline QString const& name() const {return _name;}
    inline QString const& label() const {return _label;}
    inline std::map<int,QString> const& values()  const {return _vallist;}
    inline bool usability() const {return _usability;}

};

#endif // VARIABLE_H_INCLUDED
