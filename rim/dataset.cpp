#include <dataset.h>
#include <algorithm>
#include <climits>
#include <cassert>
#include <sstream>
#include <cctype>
#include <QMessageBox>

dataset::~dataset()
{
    for (size_t i = 0; i < _varlist.size(); ++i)
        delete _varlist[i];
    for (size_t i = 0; i < _treelist.size(); ++i)
        delete _treelist[i];
}

void dataset::remove_tree(const tree* arg)
{
    assert(std::find(_treelist.cbegin(), _treelist.cend(), arg) != _treelist.cend());
    _treelist.erase(std::find(_treelist.cbegin(), _treelist.cend(), arg));
    delete arg;
}

void dataset::clear()
{
    while (!_varlist.empty())
    {
        variable* temp = _varlist.back();
        delete temp;
        _varlist.pop_back();
    }

    _data.clear();
    _id = 0;
}

tree *dataset::add_tree()
{
    tree* temp = new tree;
    _treelist.push_back(temp);
    return temp;
}

void dataset::load_variables(QString const &arg)
{
    char flag;
    process file(arg, flag);

    while (flag)
    {
        QString name, label;
        caf_type values;

        file.again();
        file.again(name);
        file.again(label);
        file.again(values);

        if (flag == 1)
            _varlist.push_back(new variable(name, label));
        else if (flag == 2)
            _varlist.push_back(new variable(name, label, values));
        if (_varlist.size() == 1)
            change_id(0);
    }
}

void dataset::load_data(QString const &arg)
{
    _data.clear();
    std::ifstream file(arg.toStdString());
    while (file)
    {
        std::string line;
        getline(file, line);
        if (line.empty())
            break;
        std::istringstream temp(line += ";");
        std::map<variable*,int> record;

        for(size_t i = 0; temp; ++i)
        {
            int buffer, code = INT_MIN;
            char peek = temp.peek();
            if (peek == ';')
                temp >> buffer;
            else
                temp >> code;
            temp.clear();
            peek = temp.get();
            if (isgraph(peek) && peek != ';')
               throw bad_file_format_exc();
            if (temp)
                record[_varlist[i]] = code;
        }

        if (record.size() != _varlist.size())
            throw bad_file_format_exc();
        _data.insert(record);
    }

    _settings.total = _data.size();
}

void dataset::outwrite(const QString &path, size_t index) const
{
    if (_weightings.empty())
        throw no_weightings_done();

    std::ofstream file(path.toStdString());

    for (data_type::const_iterator iter = _data.begin(); iter != _data.end(); ++iter)
    {
        std::vector<tree::node*> temp;
        for (size_t axis = 0; axis < _treelist.size(); ++axis)
        {
            tree::node* ptr = _treelist[axis] -> root();
            while(!ptr -> my_children().empty())
            {
                for (size_t j = 0; j < ptr -> my_children().size(); ++j)
                if (ptr->my_children().at(j)->my_value() == iter -> at(ptr -> my_next()))
                {
                    ptr = ptr -> my_children()[j];
                    break;
                }
            }

            temp.push_back(ptr);
        }

        for (size_t j = 0; j < _weightings.back().my_crosses().size(); ++j)
            if (engine::cross(temp) == _weightings.back().my_crosses()[j])
            {
                file << iter->at(_varlist[_id]) << ";" << _weightings.back().my_crosses()[j].adjustment / _weightings.back().my_crosses()[j].initial << std::endl;
                break;
            }

    }


}

void dataset::weight(rimtype &rim)
{

    QString prompt("Weighting succesful");
    try
    {
        _weightings.emplace_back(_data, _treelist, rim, _settings);
    }
    catch (std::exception const& arg)
    {
        prompt = arg.what();
    }

    QMessageBox box;
    box.setText(prompt);
    box.addButton(QString("OK"), QMessageBox::AcceptRole);
    box.exec();

}

//============= process =========================

void dataset::process::again()
{
    if (!stream.atEnd())
    {
        QString temp(stream.readLine());
        if (temp == QString("f"))
            state = 2;
        else if (temp == QString("a"))
            state = 1;
        else
            state = 0;
    }
}

void dataset::process::again(QString &temp)
{
    if (!stream.atEnd() && state)
        temp = QString(stream.readLine());
}

void dataset::process::again(caf_type &cafeteria)
{
    if (!stream.atEnd() && state)
    {
        QString temp(stream.readLine());
        while (!temp.isEmpty())
        {
            QTextStream line(&temp);
            int code = INT_MIN;
            line >> code;
            if (line.status())
                throw bad_file_format_exc();
            line.read(1);
            QString label(line.readLine());
            cafeteria[code] = label;

            temp.clear();
            temp = QString(stream.readLine());
        }
    }
    else if (!stream.atEnd())
        throw bad_file_format_exc();
    else
        state = 0;

}


