#include "VariableDescription.h"

namespace Tgs
{

VariableDescription::VariableDescription(QString name, VariableType type, double min, double max)
{
  _name = name;
  _type = type;
  _min = min;
  _max = max;
}

}
