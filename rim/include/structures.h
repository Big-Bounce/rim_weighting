#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <exception>
#include <QString>
#include <string>
#include <cstring>
#include <QException>

struct settings
{
    double rms;
    unsigned iterations;
    unsigned total;
    settings(): rms(0.5), iterations(200), total(0) {}
    settings(double arg): rms(arg) {}
};

struct volumes
{
    double actual;
    double target;
    volumes(): actual(0), target(0) {}
    volumes(const volumes &arg): actual(arg.actual), target(arg.target) {}
    volumes(double arg): actual(0), target(arg) {}
    volumes &operator= (const volumes &arg) {actual = arg.actual; target = arg.target; return *this;}
};

//===============================================
//EXCEPTIONS

struct bad_file_format_exc: public QException
{
    QString whats()  const {return "Input file's wrong format";}
};

struct no_axes_exc: public QException
{
    QString whats() const {return "No axes defined";}
};

struct undefined_value_exc: public QException
{
    std::string variable;
    int value;
    undefined_value_exc(QString const& var, int val): variable(var.toStdString()), value(val) {}
    QString whats () const {
        std::string a("Undefined value ");
        std::string b = std::to_string(value);
        std::string c = std::string(" in variable: ");
        std::string d = variable;
        const char* output = (a+b+c+d).c_str();
        return output;
    }
};

struct zero_case_exc: public QException
{
    std::vector<QString> vallabels;
    zero_case_exc(std::vector<QString> const& arg): vallabels(arg) {}
    QString whats() const {
    return "Zero case in cell: ";
    }
};

struct weighting_failed_exc: public QException
{
    double rms;
    unsigned iterations;
    weighting_failed_exc(settings arg): rms(arg.rms), iterations(arg.iterations) {}
    QString whats() const
    {
        std::string a ("Weighting failed after ");
        std::string b = std::to_string(iterations);
        std::string c (" iterations, RMS = ");
        std::string d = std::to_string(rms);
        const char* output = (a+b+c+d).c_str();
        return output;
    }
};

struct no_weightings_done: public QException
{
    QString whats()  const {return "No weightings done";}
};

#endif // STRUCTURES_H
