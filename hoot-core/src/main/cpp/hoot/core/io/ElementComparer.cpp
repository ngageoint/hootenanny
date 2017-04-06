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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementComparer.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

ElementComparer::ElementComparer(Meters threshold) :
_threshold(threshold)
{

}

bool ElementComparer::isSame(ElementPtr e1, ElementPtr e2)
{
  if (e1->getElementId() != e2->getElementId() ||
      !(e1->getTags() == e2->getTags()) ||
      e1->getStatus() != e2->getStatus() ||
      (e1->getVersion() != e2->getVersion()) ||
      fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
  {
    /*if (!(e1->getTags() == e2->getTags()))
    {
      LOG_TRACE("compare failed on tags:");
    }
    else if (e1->getStatus() != e2->getStatus())
    {
      LOG_TRACE("compare failed on status:");
    }
    else if (e1->getVersion() != e2->getVersion())
    {
      LOG_TRACE("compare failed on version:");
    }
    else if (fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
    {
      LOG_TRACE("compare failed on circular error:");
      LOG_VART(fabs(e1->getCircularError() - e2->getCircularError()));
      LOG_VART(_threshold);
    }*/
    LOG_VART(e1);
    LOG_VART(e2);
    return false;
  }
  switch (e1->getElementType().getEnum())
  {
    case ElementType::Node:
      return _compareNode(e1, e2);
    case ElementType::Way:
      return _compareWay(e1, e2);
    case ElementType::Relation:
      return _compareRelation(e1, e2);
    default:
      throw IllegalArgumentException("Unexpected element type.");
  }
}

bool ElementComparer::_compareNode(const boost::shared_ptr<const Element>& re,
                                   const boost::shared_ptr<const Element>& e)
{
  ConstNodePtr rn = dynamic_pointer_cast<const Node>(re);
  ConstNodePtr n = dynamic_pointer_cast<const Node>(e);

  LOG_VART(GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()));
  return (GeometryUtils::haversine(rn->toCoordinate(), n->toCoordinate()) <= _threshold);
}

bool ElementComparer::_compareWay(const boost::shared_ptr<const Element>& re,
                                  const boost::shared_ptr<const Element>& e)
{
  ConstWayPtr rw = dynamic_pointer_cast<const Way>(re);
  ConstWayPtr w = dynamic_pointer_cast<const Way>(e);

  if (rw->getNodeIds().size() != w->getNodeIds().size())
  {
    return false;
  }
  for (size_t i = 0; i < rw->getNodeIds().size(); ++i)
  {
    if (rw->getNodeIds()[i] != w->getNodeIds()[i])
    {
      return false;
    }
  }

  return true;
}

bool ElementComparer::_compareRelation(const boost::shared_ptr<const Element>& re,
                                       const boost::shared_ptr<const Element>& e)
{
  ConstRelationPtr rr = dynamic_pointer_cast<const Relation>(re);
  ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);

  if (rr->getType() != r->getType() ||
      rr->getMembers().size() != r->getMembers().size())
  {
    return false;
  }

  for (size_t i = 0; i < rr->getMembers().size(); i++)
  {
    if (rr->getMembers()[i].role != r->getMembers()[i].role ||
        rr->getMembers()[i].getElementId() != r->getMembers()[i].getElementId())
    {
      return false;
    }
  }

    return true;
  }
}
