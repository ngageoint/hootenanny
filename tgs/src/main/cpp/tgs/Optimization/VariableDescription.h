#ifndef VARIABLEDESCRIPTION_H
#define VARIABLEDESCRIPTION_H

// Qt
#include <QString>

// Tgs
#include <tgs/SharedPtr.h>

namespace Tgs
{
using namespace boost;

class VariableDescription
{
public:
  typedef enum
  {
    Real,
    Integer
  } VariableType;

  VariableDescription(QString name, VariableType type, double min, double max);

  VariableType getType() const { return _type; }

  double getMin() const { return _min; }

  int getMinInt() const { return (int)(_min + 0.5); }

  double getMax() const { return _max; }

  int getMaxInt() const { return (int)(_max + 0.5); }

  double getRange() const { return _max - _min; }

  QString getName() const { return _name; }

private:
  QString _name;
  double _min, _max;
  VariableType _type;
};

typedef shared_ptr<VariableDescription> VariableDescriptionPtr;
typedef shared_ptr<const VariableDescription> ConstVariableDescriptionPtr;

}

#endif // VARIABLEDESCRIPTION_H
