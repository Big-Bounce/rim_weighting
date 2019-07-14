#ifndef DATASET_H_INCLUDED
#define DATASET_H_INCLUDED

#include <vector>
#include <variable.h>
#include <tree.h>
#include <fstream>
#include <set>
#include <map>
#include <QString>
#include <unordered_map>
#include <structures.h>
#include <engine.h>
#include <QFile>
#include <QTextStream>

class dataset
{
    //aliases
    typedef std::vector<variable*> vars_type;
    typedef std::vector<tree*> axes_type;
    typedef std::set<std::map<variable*,int> > data_type;
    typedef std::map<int, QString> caf_type;
    typedef std::map<variable*,int> rec_type;
    typedef std::map<tree::node*, volumes> rimtype;

    struct process
    {
        QFile file;
        char &state;
        QTextStream stream;
        process(QString const &path, char& flag): file(path), state(flag), stream(&file) {state = file.open(QIODevice::ReadOnly | QIODevice::Text);}
        void again();
        void again(QString &temp);
        void again(caf_type &cafeteria);
        void again(rec_type &record, vars_type const &variables);
    };

    settings _settings;
    vars_type _varlist;
    axes_type _treelist;
    data_type _data;
    size_t _id;
   std::vector<engine> _weightings;

public:
    dataset(){}
    dataset(dataset const& arg): _id(0) {}
    void operator= (dataset const& arg) {}
    ~dataset();

    inline void change_id (size_t arg) {_id = arg;}
    inline size_t id() const {return _id;}
    void load_variables (QString const &arg);
    void load_data (QString const &arg);

    inline vars_type const &varlist() const {return _varlist;}
    inline axes_type const &treelist() const {return _treelist;}
    inline data_type const &data() const {return _data;}

    tree* add_tree();
    void remove_tree(const tree* arg);
    void clear();

    inline void set_rms(double rms) {_settings.rms = rms;}
    inline void set_iterations(unsigned iterations) {_settings.iterations = iterations;}
    inline void set_total(unsigned total) {_settings.total = total;}
    inline double rms() const {return _settings.rms;}
    inline unsigned iterations() const {return _settings.iterations;}
    inline unsigned total() const {return _settings.total;}
    void outwrite(const QString &path, size_t index) const;

    void weight(rimtype &rim);

};

#endif // DATASET_H_INCLUDED

