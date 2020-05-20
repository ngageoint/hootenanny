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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementComparer.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementHashVisitor.h>

namespace hoot
{

ElementComparer::ElementComparer(Meters threshold) :
_threshold(threshold),
_ignoreElementId(false)
{
}

QString ElementComparer::toHashString(const ConstElementPtr& e) const
{
  ElementHashVisitor hashVis;
  hashVis.setOsmMap(_map.get());
  // TODO: This doesn't seem right, but its been working this way for awhile now...remove later?
  hashVis.setIncludeCircularError(true);
  return hashVis.toHashString(e);
}

bool ElementComparer::tagsAreSame(ConstElementPtr e1, ConstElementPtr e2)
{
  // Considered checking tag sizes here first as an optimization, but since we're ignoring metadata
  // tags by the time they are removed, we might have well as just calculated the hash.

  // no need to set a map if just comparing tags
  ElementHashVisitor hashVis;
  return hashVis.toHashString(e1->getTags()) == hashVis.toHashString(e2->getTags());
}

bool ElementComparer::isSame(ElementPtr e1, ElementPtr e2) const
{
  if (_ignoreElementId && !_map.get())
  {
    throw IllegalArgumentException(
      "If ignoring element IDs in ElementComparer a map must be passed in.");
  }

  //LOG_VART(e1->getElementId());
  //LOG_VART(e2->getElementId());

  if (e1->getElementType() != e2->getElementType())
  {
    return false;
  }

  if (e1->getElementType() == ElementType::Node)
  {
    // Set the hoot hash tag here if it doesn't exist, since its required for node comparisons.
    ElementHashVisitor hashVis;
    hashVis.setOsmMap(_map.get());
    // TODO: This doesn't seem right, but its been working this way for awhile now...remove later?
    hashVis.setIncludeCircularError(true);
    if (!e1->getTags().contains(MetadataTags::HootHash()))
    {
      hashVis.visit(e1);
    }
    if (!e2->getTags().contains(MetadataTags::HootHash()))
    {
      hashVis.visit(e2);
    }
  }
  // only nodes have been converted over to use hash comparisons so far
  else
  {
    // not checking status here b/c if only the status changed on the element and no other tags or
    // geometries, there's no point in detecting a change
    if ((!_ignoreElementId && e1->getElementId() != e2->getElementId()) ||
        !tagsAreSame(e1, e2) || (e1->getVersion() != e2->getVersion()) ||
        fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
    {
      if (Log::getInstance().getLevel() == Log::Trace)
      {
        if ( e1->getElementId() != e2->getElementId())
        {
          LOG_TRACE("compare failed on IDs");
        }
        else if (e1->getVersion() != e2->getVersion())
        {
          LOG_TRACE("compare failed on version");
        }
        else if (fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
        {
          LOG_TRACE("compare failed on circular error:");
          LOG_VART(fabs(e1->getCircularError() - e2->getCircularError()));
          LOG_VART(_threshold);
        }

        LOG_TRACE(
          "elements failed comparison: " << e1->getElementId() << ", " << e2->getElementId());
      }

      return false;
    }
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

bool ElementComparer::_compareNode(const std::shared_ptr<const Element>& re,
                                   const std::shared_ptr<const Element>& e) const
{
  if (!re->getTags().contains(MetadataTags::HootHash()) ||
      !e->getTags().contains(MetadataTags::HootHash()))
  {
    throw HootException(
      QString("ElementComparer requires the %1 tag be set for node comparison. Nodes: %2, %3")
      .arg(MetadataTags::HootHash())
      .arg(re->getElementId().toString())
      .arg(e->getElementId().toString()));
  }

  if (re->getElementId().getId() == DEBUG_ID || e->getElementId().getId() == DEBUG_ID)
  {
    LOG_VART(re);
    LOG_VART(e);
  }

  bool same = false;
  if (re->getTags()[MetadataTags::HootHash()] == e->getTags()[MetadataTags::HootHash()])
  {
    LOG_TRACE("Nodes " << re->getElementId() << " and " << e->getElementId() << " are the same.");
    same = true;
  }
  else
  {
    LOG_TRACE("Compare failed:");
    LOG_VART(re);
    LOG_VART(e);
  }

  return same;
}

bool ElementComparer::_compareWay(const std::shared_ptr<const Element>& re,
                                  const std::shared_ptr<const Element>& e) const
{
  ConstWayPtr rw = std::dynamic_pointer_cast<const Way>(re);
  ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);

  if (rw->getNodeIds().size() != w->getNodeIds().size())
  {
    LOG_TRACE(
      "Ways " << rw->getElementId() << " and " << w->getElementId() <<
      " failed comparison on way node count: " << rw->getNodeIds().size() << " and " <<
      w->getNodeIds().size());
    return false;
  }

  if (!_ignoreElementId)
  {
    for (size_t i = 0; i < rw->getNodeIds().size(); ++i)
    {
      if (rw->getNodeIds()[i] != w->getNodeIds()[i])
      {
        LOG_TRACE(
          "Ways " << rw->getElementId() << " and " << w->getElementId() <<
          " failed comparison on way nodes: " << rw->getNodeIds() << " and " << w->getNodeIds());
        return false;
      }
    }
  }
  else
  {
    for (size_t i = 0; i < rw->getNodeIds().size(); ++i)
    {
      NodePtr nodeRw = _map->getNode(ElementId(ElementType::Node, rw->getNodeIds()[i]));
      NodePtr nodeW = _map->getNode(ElementId(ElementType::Node, w->getNodeIds()[i]));
      if (!nodeRw || !nodeW || !isSame(nodeRw, nodeW))
      {
        LOG_TRACE(
          "Ways " << rw->getElementId() << " and " << w->getElementId() <<
          " failed comparison on way nodes: " << rw->getNodeIds() << " and " << w->getNodeIds());
        return false;
      }
    }
  }

  LOG_TRACE("Ways " << re->getElementId() << " and " << e->getElementId() << " are the same.");
  return true;
}

bool ElementComparer::_compareRelation(const std::shared_ptr<const Element>& re,
                                       const std::shared_ptr<const Element>& e) const
{
  ConstRelationPtr rr = std::dynamic_pointer_cast<const Relation>(re);
  ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);

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

  LOG_TRACE("Relations " << re->getElementId() << " and " << e->getElementId() << " are the same.");
  return true;
}

}
