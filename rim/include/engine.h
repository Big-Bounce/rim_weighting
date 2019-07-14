#ifndef ENGINE_H
#define ENGINE_H

#include <tree.h>
#include <vector>
#include <unordered_map>
#include <list>
#include <cmath>
#include <set>
#include <structures.h>

class engine
{

    //aliases
    typedef std::vector<tree::node*> crosstype;
    typedef std::set<std::map<variable*,int> > datatype;
    typedef std::vector<std::list<tree::node*>::const_iterator> spacetype;
    typedef std::vector<tree::node*>::const_iterator childitertype;
    typedef std::map<tree::node*, volumes> rimtype;

public:

    struct cross
    {
        crosstype coordinates;
        double adjustment;
        double initial;
        cross(crosstype const& arg): coordinates(arg), adjustment(0), initial(0) {}
        ~cross(){}
        bool operator== (cross const& arg) {return coordinates == arg.coordinates;}
    };


    struct adjust_cross
    {

        size_t current;
        rimtype &rim;
        adjust_cross(rimtype &cells, size_t axis): current(axis), rim(cells) {}
        void operator() (cross &);
    };

    struct read_adjustment
    {
        bool clearing;
        rimtype &rim;
        double rms_component;
        read_adjustment(rimtype &cells, bool flag = false): clearing(flag), rim(cells), rms_component(0) {}
        void operator() (tree::node*);
    };

    std::vector<cross> _crosses;

public:
    engine(){}
    engine(datatype const &data, std::vector<tree*> const &axes, rimtype &rim, settings limits);
    void operator= (engine const &arg) {};
    ~engine(){}

    inline const std::vector<cross> &my_crosses() const {return _crosses;}
};

#endif // ENGINE_H
