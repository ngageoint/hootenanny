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

ElementComparer::ElementComparer() :
_ignoreElementId(false),
_ignoreVersion(false)
{
}

QString ElementComparer::toHashString(const ConstElementPtr& e) const
{
  if (!e)
  {
    return "";
  }

  ElementHashVisitor hashVis;
  hashVis.setOsmMap(_map.get());
  // TODO: This doesn't seem right, but its been working this way for awhile now...remove later?
  hashVis.setIncludeCircularError(true);
  return hashVis.toHashString(e);
}

bool ElementComparer::isSame(ElementPtr e1, ElementPtr e2) const
{
  if (!e1 || !e2)
  {
    return false;
  }

  // different types?
  if (e1->getElementType() != e2->getElementType())
  {
    LOG_TRACE("compare failed on type: " << e1->getElementId() << ", " << e2->getElementId());
    return false;
  }

  // different IDs?
  if (!_ignoreElementId && (e1->getElementId() != e2->getElementId()))
  {
    LOG_TRACE("compare failed on ID: " << e1->getElementId() << ", " << e2->getElementId());
    return false;
  }

  // different versions?
  if (!_ignoreVersion && (e1->getVersion() != e2->getVersion()))
  {
    LOG_TRACE("compare failed on version: " << e1->getElementId() << ", " << e2->getElementId());
    return false;
  }

  if (_ignoreElementId && !_map.get())
  {
    throw IllegalArgumentException(
      "If ignoring element IDs in ElementComparer a map must be passed in.");
  }

  LOG_VART(e1->getElementId());
  LOG_VART(e2->getElementId());

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

void ElementComparer::_setHash(ElementPtr element) const
{
  // Set the hoot hash tag here if it doesn't exist.
  ElementHashVisitor hashVis;
  hashVis.setOsmMap(_map.get());
  // TODO: This doesn't seem right, but its been working this way for awhile now...remove later?
  hashVis.setIncludeCircularError(true);
  if (!element->getTags().contains(MetadataTags::HootHash()))
  {
    hashVis.visit(element);
  }
}

bool ElementComparer::_compareNode(ElementPtr re, ElementPtr e) const
{
  LOG_TRACE("Comparing nodes...");

  _setHash(re);
  _setHash(e);

  if (re->getElementId().getId() == DEBUG_ID || e->getElementId().getId() == DEBUG_ID)
  {
    LOG_VART(re);
    LOG_VART(e);
  }

  return _haveSameHash(re, e);
}

bool ElementComparer::_compareWay(ElementPtr re, ElementPtr e) const
{
  LOG_TRACE("Comparing ways...");

  ConstWayPtr rw = std::dynamic_pointer_cast<const Way>(re);
  ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);

  // optimization; tag count comparison isn't a good opt b/c you need to count metadata tags only
  // and that count isn't available without extra computation
  if (rw->getNodeCount() != w->getNodeCount())
  {
    LOG_TRACE(
      "Ways " << rw->getElementId() << " and " << w->getElementId() <<
      " failed comparison on way node count: " << rw->getNodeCount() << " and " <<
      w->getNodeCount());
    return false;
  }

  if (!_ignoreElementId)
  {
    // If we're not ignoring the element IDs of the way nodes, then we'll perform just these checks
    // instead of computing a hash and avoid having to use a passed in map for callers that don't
    // have one.

    if (!rw->hasSameNodes(*w))
    {
      LOG_TRACE(
        "Ways " << rw->getElementId() << " and " << w->getElementId() <<
        " failed comparison on way nodes: " << rw->getNodeIds() << " and " << w->getNodeIds());
      return false;
    }

    if (!rw->hasSameNonMetadataTags(*w))
    {
      LOG_TRACE(
        "Ways " << rw->getElementId() << " and " << w->getElementId() <<
        " failed comparison on tags.");
      return false;
    }

    LOG_TRACE("Ways " << re->getElementId() << " and " << e->getElementId() << " are the same.");
    return true;
  }
  else
  {
    // If we're ignoring element ID, then we have to dig deeper and compare each way node, which the
    // hashing takes care of. Hashing also takes tags into account.
    _setHash(re);
    _setHash(e);
    return _haveSameHash(re, e);
  }
}

bool ElementComparer::_compareRelation(ElementPtr re, ElementPtr e) const
{
  LOG_TRACE("Comparing relations...");

  ConstRelationPtr rr = std::dynamic_pointer_cast<const Relation>(re);
  ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);

  // optimization; tag count comparison isn't a good opt b/c you need to count metadata tags only
  // and that count isn't available without extra computation
  if (rr->getType() != r->getType())
  {
    LOG_TRACE(
      "Relations " << rr->getElementId() << " and " << r->getElementId() <<
      " failed comparison on type: " << rr->getType() << " and " << r->getType());
    return false;
  }
  if (rr->getMemberCount() != r->getMemberCount())
  {
    LOG_TRACE(
      "Relations " << rr->getElementId() << " and " << r->getElementId() <<
      " failed comparison on relation member count: " << rr->getMemberCount() << " and " <<
      r->getMemberCount());
    return false;
  }

  if (!_ignoreElementId)
  {
    // If we're not ignoring the element IDs of the relation members, then we'll perform just these
    // checks instead of computing a hash and avoid having to use a passed in map for callers that
    // don't have one.

    if (!rr->hasSameNonMetadataTags(*r))
    {
      LOG_TRACE(
        "Relations " << rr->getElementId() << " and " << r->getElementId() <<
        " failed comparison on tags.");
      return false;
    }

    for (size_t i = 0; i < rr->getMembers().size(); i++)
    {
      if (rr->getMembers()[i].role != r->getMembers()[i].role ||
          rr->getMembers()[i].getElementId() != r->getMembers()[i].getElementId())
      {
        LOG_TRACE(
          "Relations " << rr->getElementId() << " and " << r->getElementId() <<
          " failed comparison on relation members.");
        return false;
      }
    }
    return true;
  }
  else
  {
    // If we're ignoring element ID, then we have to dig deeper and compare each member element,
    // which the hashing takes care of. Hashing also takes tags into account.
    _setHash(re);
    _setHash(e);
    return _haveSameHash(re, e);
  }
}

bool ElementComparer::_haveSameHash(ElementPtr re, ElementPtr e) const
{
  if (!re->getTags().contains(MetadataTags::HootHash()) ||
      !e->getTags().contains(MetadataTags::HootHash()))
  {
    throw HootException(
      QString("ElementComparer requires the %1 tag be set for element comparison. Elements: %2, %3")
        .arg(MetadataTags::HootHash())
        .arg(re->getElementId().toString())
        .arg(e->getElementId().toString()));
  }

  bool same = false;
  if (re->getTags()[MetadataTags::HootHash()] == e->getTags()[MetadataTags::HootHash()])
  {
    LOG_TRACE(re->getElementId() << " and " << e->getElementId() << " are the same.");
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

}
