#include "include/tree.h"
#include <cassert>
#include <algorithm>

//********************************* NODE ******************************************

tree::node::node(): parent(nullptr), next(nullptr), skewcount(0)
{
}

tree::node::node(node *arg_parent, int arg_value): parent(arg_parent), value(arg_value), next(nullptr), skewcount(0)
{
}

tree::node::node(node *arg_parent, node *arg_sibling, cells_type &arg_cells):
    parent(arg_parent),
    value(arg_sibling -> value),
    next(arg_sibling -> next),
    skewcount(arg_sibling -> skewcount)
{
    for (children_iter iter = arg_sibling->children.begin(); iter != arg_sibling->children.end(); ++iter)
        children.push_back(new node(this, *iter, arg_cells));

    if (children.empty())
        arg_cells.push_back(this);
}

tree::node::~node()
{
    for (children_iter iter = children.begin(); iter != children.end(); ++iter)
        delete *iter;
}

bool tree::node::is_moderable() const
{
   const node* temp = this;
   while (temp -> parent && !temp->skewcount)
       temp = temp -> parent;
   return temp->skewcount == 1;

}

//=================================================================================
//recursive methods

void tree::node::erase_nodes(variable* arg, cells_type  &arg_cells)
{

    if (next != arg)
    {
        for (children_iter iter = children.begin(); iter != children.end(); ++iter)
            (*iter) -> erase_nodes(arg, arg_cells);
        return;
    }

    for (children_iter iter = children.begin(); iter != children.end(); ++iter)
       if (iter != children.begin())
       {
           (*iter)  -> erase_subtree_cells(arg_cells);
           delete *iter;
       }
       else
          arg_cells.remove(*iter); //this node's children do not need to be cells


        //aquiring this node's first child's children (could be empty set)
        node* pointer = *children.begin();
        children = pointer -> children;
        next = pointer -> next;
        pointer->children.clear();
        delete pointer;

        for (children_iter iter = children.begin(); iter != children.end(); ++iter)
            (*iter) -> parent = this;
        //this node becomes a cell, if this node's children were cells up to now
        if (parent && children.empty())
            arg_cells.push_back(this);    
}

void tree::node::erase_subtree_cells(cells_type &arg)
{
    if (!children.empty())
        for (children_iter iter = children.begin(); iter != children.end(); ++iter)
            (*iter) -> erase_subtree_cells(arg);
    else
        arg.remove(this);
}

void tree::node::find_down (variable* var, bool &found)
{
    if (next == var)
        found = true;
    else
        for (children_iter iter = children.begin(); iter != children.end(); ++iter)
            (*iter) -> find_down(var, found);
}

void tree::node::find_up(variable *var, bool &found)
{
    if (next == var)
        found = true;
    else if (parent != nullptr)
        parent -> find_up(var, found);
}

//**************************** TREE ***********************************************

tree::tree(): _root(new node), _skewcount(0)
{
}

tree::~tree()
{
    delete _root;
}

void tree::add_level(variable *var, node *hook)
{
    if (!hook)
        hook = _root;
    bool found = false;

    if (hook == _root)
    {
        hook -> find_down(var, found);
        if (found)
            return;
        //if I am to add level to the root, I am multiplying tree's skewness
        _skewcount *= var->values().size();
    }

    else
    {
        hook -> find_up(var, found);
        if (found)
            return;
        cells_iter cell = std::find(_cells.begin(), _cells.end(), hook);
        if (cell != _cells.end())
        {
            //if i am to add level to the cell, I'am increasing cell's skewness, its parent (optionally) and removing cell from cells
            ++_skewcount;
            hook -> skewcount = 1;
            if (hook -> parent -> skewcount)
                ++(hook -> parent -> skewcount);
            _cells.erase(cell);
         }
    }

    //===================================

    children_type help = hook -> children;
    hook->children.clear();

    for (values_iter iter = var->values().cbegin(); iter != var->values().cend(); ++iter)
    {
        //new level
        node* pointer = new node(hook, iter -> first);
        hook->children.push_back(pointer);
        pointer -> next = hook -> next;

        if (iter == var->values().cbegin())
        {
            //adding foregoing hook's children to the first node of new level
            pointer -> children = help;
            for (children_iter i = help.begin(); i != help.end(); ++i)
                (*i) -> parent = pointer;
        }
        else
            //multiplying nodes and adding to the remaining nodes of new level
            for (children_iter i = help.begin(); i != help.end(); ++i)
                pointer->children.push_back(new node(pointer, *i, _cells));

        //if I'am adding level to cell, recursive node::node() has not been invoked, so I need to add cells manually
        if (pointer->children.empty())
            _cells.push_back(pointer);
    }

    hook -> next = var;
}

tree::node* tree::find_subtree_root(tree::node* hook)
{
    //should be invoked for cells only
    assert(std::find(_cells.cbegin(), _cells.cend(), hook) != _cells.cend());
    while(hook -> parent && !hook->skewcount)
        hook = hook -> parent;
    return hook;
}

void tree::erase_level(variable *var, node *hook)
{
    if (!hook)
        hook = _root;

    //if I'm in root, tree must not be skewed. If I'm not in root, node's skewness must be equal to 1
    assert((hook == _root && !_skewcount) || hook -> skewcount == 1);

    hook -> erase_nodes(var, _cells);
    if (hook->children.empty() && hook != _root)
    {
        //if I have erased level from cells, I need to remove skewness of hook and its parent (optionally)
        hook -> skewcount = 0;
        if (hook -> parent -> skewcount)
            --(hook -> parent -> skewcount);
        --_skewcount;
        //I am not adding hook to cells - erase_nodes method will do it automatically
     }
}

void tree::clear()
{
    delete _root;
    _root = new node;
    _cells.clear();
    _skewcount = 0;
}
