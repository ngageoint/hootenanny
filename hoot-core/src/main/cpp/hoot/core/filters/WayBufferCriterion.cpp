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

#include "WayBufferCriterion.h"

// GEOS
#include <geos/geom/Geometry.h>
#include <geos/operation/buffer/BufferOp.h>
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/ElementConverter.h>

// Qt
#include <QDebug>

namespace hoot
{

WayBufferCriterion::WayBufferCriterion(ConstOsmMapPtr map,
                                       ConstWayPtr baseWay,
                                       Meters buffer,
  double matchPercent) :
  _map(map)
{
  _matchPercent = matchPercent;
  _buffer = buffer + baseWay->getCircularError();
  _baseLs = ElementConverter(map).convertToLineString(baseWay);
  _baseLength = _baseLs->getLength();
  _bufferAccuracy = -1;
}

WayBufferCriterion::WayBufferCriterion(ConstOsmMapPtr map,
                                       shared_ptr<LineString> baseLine,
                                       Meters buffer,
  Meters circularError, double matchPercent) :
  _map(map)
{
  _matchPercent = matchPercent;
  _buffer = buffer + circularError;
  _baseLs = baseLine;
  _baseLength = _baseLs->getLength();
  _bufferAccuracy = -1;
}

bool WayBufferCriterion::isSatisfied(const shared_ptr<const Element> &e) const
{
  if (e->getElementType() != ElementType::Way)
    return false;

  ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
  try
  {
    bool result = true;
    shared_ptr<LineString> ls2 = ElementConverter(_map).
        convertToLineString(_map->getWay(w->getId()));

    if (fabs((w->getCircularError() + _buffer) - _bufferAccuracy) > 0.1)
    {
      _bufferAccuracy = w->getCircularError() + _buffer;

      _baseBuffered.reset(_baseLs->buffer(_bufferAccuracy, 3,
                                          geos::operation::buffer::BufferOp::CAP_ROUND));
      _boundsPlus = *_baseBuffered->getEnvelopeInternal();
    }

    if (ls2->getEnvelopeInternal()->intersects(_boundsPlus))
    {

      shared_ptr<Geometry> g(_baseBuffered->intersection(ls2.get()));
      double ls2Length = ls2->getLength();
      double ls2IntersectLength = g->getLength();

      if (ls2IntersectLength / ls2Length >= _matchPercent)
      {
        shared_ptr<Geometry> ls2Buffer(ls2->buffer(_bufferAccuracy, 3,
                                                   geos::operation::buffer::BufferOp::CAP_ROUND));
        g.reset(ls2Buffer->intersection(_baseLs.get()));
        double ls1IntersectLength = g->getLength();

        if (ls1IntersectLength / _baseLength >= _matchPercent)
        {
          result = false;
        }
      }
    }

    return result;
  }
  catch (geos::util::TopologyException& e)
  {
    LOG_WARN(ElementConverter(_map).convertToLineString(_map->getWay(w->getId())));
    throw e;
  }
}

}
