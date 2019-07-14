#include <engine.h>
#include <cassert>
#include <exception>
#include <vector>
#include <QString>

void engine::adjust_cross::operator() (cross &arg)
{
    unsigned target = rim[arg.coordinates[current]].target, actual = rim[arg.coordinates[current]].actual;
    if (target && !actual)
    {
        std::vector<QString> vallabels;
        for (size_t i  = 0; i < arg.coordinates.size(); ++ i)
            vallabels.push_back(arg.coordinates[i]->my_parent()->my_next()->values().at(arg.coordinates[i]->my_value()));
        throw zero_case_exc(vallabels);

    }

    arg.adjustment *= (actual ? double(target) / double(actual) : 0);
    if (arg.coordinates.size() > 1)
    {
        size_t temp = (current == 0 ? arg.coordinates.size() - 1 : current - 1);
        rim[arg.coordinates[temp]].actual += arg.adjustment;

        if (arg.coordinates.size() > 2)
        {
            temp = (current == arg.coordinates.size() - 1 ? 0 : current + 1);
            rim[arg.coordinates[temp]].actual += arg.adjustment;
        }
    }
}

void engine::read_adjustment::operator() (tree::node* arg)
{
    if (clearing)
        rim[arg].actual = 0;
    else
        rms_component += pow(rim[arg].target - rim[arg].actual, 2);
}

engine::engine(datatype const &data, std::vector<tree*> const &axes, rimtype &rim, settings limits)
{
    if (axes.empty())
           throw no_axes_exc();

    spacetype elements(axes.size());
    for (size_t i = 0; i < elements.size(); ++i)
        elements[i] = (axes[i] -> cells()).cbegin();
    spacetype cycle(elements);

    do
    {
        crosstype make_cross(axes.size());
         for (size_t i = 0; i < make_cross.size(); ++i)
            make_cross[i] = *elements[i];
        _crosses.emplace_back(make_cross);

       size_t i = 0;
        while (i < elements.size() && ++elements[i] == (axes[i] -> cells()).cend())
        {
            elements[i] = (axes[i] -> cells()).cbegin();
            ++i;
        }

    } while (elements != cycle);

    //====================================================
    //counting actuals

    for (datatype::const_iterator dataiter = data.cbegin(); dataiter != data.cend(); ++dataiter)
    {
        crosstype pattern;
        for (size_t axiter = 0; axiter < axes.size(); ++axiter)
        {
            tree::node* drop = axes[axiter] -> root();
            while (!(drop -> my_children()).empty())
            {
                childitertype childiter = find_if(drop->my_children().cbegin(), drop -> my_children().cend(), tree::node_predicat(dataiter -> at(drop -> my_next())));
                 if (childiter != (drop -> my_children()).cend())
                    drop = *childiter;
                else
                    throw undefined_value_exc(drop -> my_next() -> name(), dataiter -> at(drop -> my_next()));
            }

            pattern.push_back(drop);
        }

        assert(find(_crosses.begin(), _crosses.end(), pattern) != _crosses.end());
        std::vector<cross>::iterator init = find(_crosses.begin(), _crosses.end(), pattern);
        init -> initial = ++ (init -> adjustment);
        ++((rim[pattern[0]]).actual);
    }

    //initial adjustment
    std::for_each(_crosses.begin(), _crosses.end(), adjust_cross(rim, 0));
    if (axes.size() < 2)
        return;


    //====================================================
    //weighting

    double rms;
    unsigned iteration = 0;
    size_t next = 1, current = 0, previous = axes.size() - 1;

    do
    {
        rms = 0;
        ++iteration;

        if (++next == axes.size())
            next = 0;
        if (++current == axes.size())
            current = 0;
        if (++previous == axes.size())
            previous = 0;

        //clearing next
        for_each(axes[next]->cells().cbegin(), axes[next]->cells().cend(), read_adjustment(rim, true));
        //reading current (before adjustment)
        rms += for_each(axes[current]->cells().cbegin(), axes[current]->cells().cend(), read_adjustment(rim)).rms_component;
        //clearing previous (if necessery)
        if (previous != next)
            for_each(axes[previous]->cells().cbegin(), axes[previous]->cells().cend(), read_adjustment(rim, true));
        //adjusting current
        for_each(_crosses.begin(), _crosses.end(), adjust_cross(rim, current));
        //reading previous (adjusted previously)
        rms += for_each(axes[previous]->cells().cbegin(), axes[previous]->cells().cend(), read_adjustment(rim)).rms_component;


    } while (rms > limits.rms && iteration < limits.iterations);

    if (rms > limits.rms)
        throw weighting_failed_exc(rms);

}

