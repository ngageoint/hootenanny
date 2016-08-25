#include "State.h"

// Qt
#include <QStringList>

namespace Tgs
{

State::State()
{
}

QString State::toString() const
{
  QStringList result;

  foreach (QString k, _values.keys())
  {
    result << QString("'%1':%2").arg(k).arg(_values[k]);
  }

  return "{" + result.join(", ") + "}";
}

}
