#include "EdgeSublineMatch.h"

namespace hoot
{

QString EdgeSublineMatch::toString() const
{
  return QString("{subline1: %1, subline2: %2}").
    arg(getSubline1()->toString()).
    arg(getSubline2()->toString());
}

}
