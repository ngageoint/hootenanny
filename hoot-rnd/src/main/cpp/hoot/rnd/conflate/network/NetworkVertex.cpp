#include "NetworkVertex.h"

namespace hoot
{

QString NetworkVertex::toString() const
{
  QString result;
  if (!_e.get())
  {
    result = "<null>";
  }
  else
  {
    result = _e->getElementId().toString();
  }

  return result;
}

}
