#include "WayString.h"

namespace hoot
{

// if the difference is smaller than this we consider it to be equivalent.
Meters WayString::_epsilon = 1e-9;

WayString::WayString()
{
}

void WayString::append(const WaySubline& subline)
{
  if (_sublines.size() > 0)
  {
    if (back().getWay() == subline.getWay() &&
      back().getEnd() != subline.getStart())
    {
      throw IllegalArgumentException("All consecutive ways must connect end to start in a "
        "WayString.");
    }
    else
    {
      if (subline.getStart().isExtreme(WayLocation::SLOPPY_EPSILON) == false ||
        back().getEnd().isExtreme(WayLocation::SLOPPY_EPSILON) == false)
      {
        throw IllegalArgumentException("If ways are different they must connect at an extreme "
          "node.");
      }
      if (back().getEnd().getNode() != subline.getStart().getNode())
      {
        LOG_VAR(back());
        LOG_VAR(subline);
        throw IllegalArgumentException("Ways must connect at a node in the WayString.");
      }
    }
  }
  _sublines.append(subline);
}

WayLocation WayString::calculateLocationFromStart(Meters distance, ElementId preferredEid) const
{
  if (distance <= 0.0)
  {
    return _sublines[0].getStart();
  }

  Meters soFar = 0.0;
  for (int i = 0; i < _sublines.size(); ++i)
  {
    Meters ls = _sublines[i].calculateLength();
    if (distance <= soFar + ls)
    {
      ConstOsmMapPtr map = _sublines[i].getStart().getMap();
      ConstWayPtr w = _sublines[i].getStart().getWay();
      Meters offset = _sublines[i].getStart().calculateDistanceOnWay();
      if (_sublines[i].isBackwards())
      {
        offset -= distance - soFar;
      }
      else
      {
        offset += distance - soFar;
      }
      // if the offset isn't expected (while allowing for floating point rounding)
      if (offset < _sublines[i].getFormer().calculateDistanceOnWay() - _epsilon ||
        offset > _sublines[i].getLatter().calculateDistanceOnWay() + _epsilon)
      {
        throw InternalErrorException("Expected offset to be in the bounds of the subline. "
          "Logic error?");
      }
      WayLocation result(map, w, offset);
      return _changeToPreferred(i, result, preferredEid);
    }
    soFar += ls;
  }

  return _sublines.back().getEnd();
}

Meters WayString::calculateDistanceOnString(const WayLocation& l) const
{
  Meters d = 0.0;

  for (int i = 0; i < _sublines.size(); ++i)
  {
    if (_sublines[i].contains(l))
    {
      d += fabs(l.calculateDistanceOnWay() - _sublines[i].getStart().calculateDistanceOnWay());
      return d;
    }
    else
    {
      d += _sublines[i].calculateLength();
    }
  }

  throw IllegalArgumentException("Way location was not found in this way string.");
}

Meters WayString::calculateLength() const
{
  Meters result = 0.0;
  for (int i = 0; i < _sublines.size(); ++i)
  {
    result += _sublines[i].getLength();
  }

  return result;
}

WayLocation WayString::_changeToPreferred(int index, const WayLocation& wl, ElementId preferredEid)
  const
{
  WayLocation result = wl;

  if (preferredEid.isNull() || wl.getWay()->getElementId() == preferredEid)
  {
    result = wl;
  }
  else if (index >= 1 &&
    _sublines[index - 1].getWay()->getElementId() == preferredEid &&
    fabs(calculateDistanceOnString(wl) - calculateDistanceOnString(_sublines[index - 1].getEnd()))
      < _epsilon)
  {
    result = _sublines[index - 1].getEnd();
  }
  else if (index < _sublines.size() - 1 &&
    _sublines[index + 1].getWay()->getElementId() == preferredEid &&
    fabs(calculateDistanceOnString(wl) - calculateDistanceOnString(_sublines[index + 1].getStart()))
      < _epsilon)
  {
    result = _sublines[index + 1].getStart();
  }

  return result;
}

void WayString::visitRo(const ElementProvider &map, ElementVisitor& v) const
{
  // go through each subline and call visitRw on the subline. The sublines should only visit the
  // nodes that intersect the subline.
  for (int i = 0; i < _sublines.size(); ++i)
  {
    _sublines[i].visitRo(map, v);
  }
}

}
