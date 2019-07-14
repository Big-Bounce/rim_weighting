#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "variable.h"
#include <vector>
#include <string>
#include <QString>
#include <list>

class tree
{

public:
    class node;

    //aliases
    typedef std::vector<node*> children_type;
    typedef std::list<node*> cells_type;
    typedef std::map<int,QString> values_type;
    typedef std::vector<node*>::iterator children_iter;
    typedef std::list<node*>::iterator cells_iter;
    typedef std::map<int,QString>::const_iterator values_iter;

    class node
    {
        friend tree;

        node *parent;
        int value;
        variable *next;
        children_type children;
        unsigned skewcount;

    public:
        node();
        node(node const &arg) {}
        node(node *arg_parent, int arg_value);
        node(node *arg_parent, node *arg_sibling, cells_type &arg_cells);
        void operator= (node const &arg) {}
        ~node();

        inline const node* my_parent() const {return parent;}
        inline variable* my_next() const {return next;}
        inline int my_value() const {return value;}
        inline const children_type &my_children() const {return children;}
        bool is_moderable() const;

    private:
        void erase_nodes (variable* arg, cells_type &arg_cells);
        void erase_subtree_cells (cells_type &arg);
        void find_up(variable* var, bool &found);
        void find_down(variable* var, bool &found);
    };

    struct node_predicat
    {
        int value;
        node_predicat(int arg): value(arg) {}
        inline bool operator() (node* arg) {return arg -> value == value;}
    };

private:

    //members
    node *_root;
    cells_type _cells;
    unsigned _skewcount;

public:

    tree();
    tree(tree const& arg) {}
    void operator= (tree const& arg) {}
    ~tree();

    void add_level(variable* var, node* hook = nullptr);
    void erase_level(variable* var, node* hook = nullptr);
    void clear();
    tree::node* find_subtree_root(tree::node* hook);

    inline const cells_type &cells() {return _cells;}
    inline bool is_skewed() const {return _skewcount;}
    inline node* root() const {return _root;}
};

#endif // TREE_H_INCLUDED
