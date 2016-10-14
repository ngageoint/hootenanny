/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EdgeSubline.h"

namespace hoot
{

bool operator==(ConstEdgeSublinePtr es1, ConstEdgeSublinePtr es2)
{
  bool result = false;

  if (es1.get() == es2.get())
  {
    result = true;
  }
  else if (es1->getStart() == es2->getStart() && es1->getEnd() == es2->getEnd())
  {
    result = true;
  }

//  bool strResult = es1->toString() == es2->toString();

//  if (result != strResult)
//  {
//    LOG_VARE(result);
//    LOG_VARE(strResult);
//    LOG_VARE(es1);
//    LOG_VARE(es2);
//    throw HootException();
//  }

  return result;
}

EdgeSubline::EdgeSubline(ConstEdgeLocationPtr start, ConstEdgeLocationPtr end) :
  _start(start),
  _end(end)
{
  //LOG_VARD(_start->getEdge());
  //LOG_VARD(_end->getEdge());
  assert(_start->getEdge() == _end->getEdge());
}

EdgeSubline::EdgeSubline(ConstNetworkEdgePtr e, double start, double end) :
  _start(new EdgeLocation(e, start)),
  _end(new EdgeLocation(e, end))
{
}

shared_ptr<EdgeSubline> EdgeSubline::clone() const
{
  return shared_ptr<EdgeSubline>(new EdgeSubline(_start, _end));
}

bool EdgeSubline::contains(ConstNetworkVertexPtr v) const
{
  bool result = false;

  if (getStart()->isExtreme() && getStart()->getVertex() == v)
  {
    result = true;
  }
  else if (getEnd()->isExtreme() && getEnd()->getVertex() == v)
  {
    result = true;
  }

  return result;
}

shared_ptr<EdgeSubline> EdgeSubline::createFullSubline(ConstNetworkEdgePtr e)
{
  return shared_ptr<EdgeSubline>(new EdgeSubline(e, 0.0, 1.0));
}

bool EdgeSubline::overlaps(shared_ptr<const EdgeSubline> other) const
{
  bool result = false;

  if (other->getEdge() == getEdge())
  {
    /// this    *---f----l----*
    /// other   *-----f----l--*
    if (other->getFormer() < getLatter() && other->getLatter() >= getLatter())
    {
      result = true;
    }
    /// this    *-----f----l--*
    /// other   *--f----l-----*
    else if (other->getFormer() <= getFormer() && other->getLatter() > getFormer())
    {
      result = true;
    }
  }

  return result;
}

QString EdgeSubline::toString() const
{
  return QString("{ _start: %1, _end: %2 }").arg(hoot::toString(_start)).arg(hoot::toString(_end));
}

}
