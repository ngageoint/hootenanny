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
#include <hoot/core/util/MetadataTags.h>

namespace hoot
{

ElementComparer::ElementComparer(Meters threshold) :
_threshold(threshold)
{
}

void ElementComparer::_removeTagsNotImportantForComparison(Tags& tags) const
{
  tags.remove(MetadataTags::HootStatus());
  //TODO: not sure where "status" is coming from...should be "hoot:status"...bug somewhere?
  tags.remove("status");
  //this is ok b/c we have the actual id to compare to, but it should still probably be fixed to
  //carry along the hoot:id tag for consistency's sake when that is desired
  tags.remove(MetadataTags::HootId());
  tags.remove(MetadataTags::SourceDateTime());
}

bool ElementComparer::isSame(ElementPtr e1, ElementPtr e2) const
{
  LOG_VART(e1->getElementId());
  LOG_VART(e2->getElementId());

  assert(e1->getElementType() == e2->getElementType());

  //only nodes have been converted over to use hash comparisons so far
  if (e1->getElementType() != ElementType::Node)
  {
    //create modified copies of the tags for comparing, as we don't care if some tags are identical
    Tags tags1 = e1->getTags();
    _removeTagsNotImportantForComparison(tags1);
    Tags tags2 = e2->getTags();
    _removeTagsNotImportantForComparison(tags2);

    // not checking status here b/c if only the status changed on the element and no other tags or
    // geometries, there's no point in detecting a change
    if (e1->getElementId() != e2->getElementId() ||
        !(tags1 == tags2) ||
        (e1->getVersion() != e2->getVersion()) ||
        fabs(e1->getCircularError() - e2->getCircularError()) > _threshold)
    {
      if (Log::getInstance().getLevel() == Log::Trace)
      {
        if (!(tags1 == tags2))
        {
          LOG_TRACE("compare failed on tags");
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

        LOG_VART(tags1);
        LOG_VART(tags2);
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

bool ElementComparer::_compareNode(const boost::shared_ptr<const Element>& re,
                                   const boost::shared_ptr<const Element>& e) const
{
  ConstNodePtr rn = boost::dynamic_pointer_cast<const Node>(re);
  ConstNodePtr n = boost::dynamic_pointer_cast<const Node>(e);

  if (!rn->getTags().contains(MetadataTags::HootHash()) ||
      !n->getTags().contains(MetadataTags::HootHash()))
  {
    throw HootException(
      "ElementComparer requires the " + MetadataTags::HootHash() +
      " tag be set for node comparison.");
  }

  bool same = false;
  if (rn->getTags()[MetadataTags::HootHash()] == n->getTags()[MetadataTags::HootHash()])
  {
    same = true;
    //LOG_TRACE("Compare succeeded:");
  }
  else
  {
    LOG_TRACE("Compare failed:");
  }
  if (!same)
  {
    LOG_VART(re);
    LOG_VART(e);
  }

  return same;
}

bool ElementComparer::_compareWay(const boost::shared_ptr<const Element>& re,
                                  const boost::shared_ptr<const Element>& e) const
{
  ConstWayPtr rw = boost::dynamic_pointer_cast<const Way>(re);
  ConstWayPtr w = boost::dynamic_pointer_cast<const Way>(e);

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
                                       const boost::shared_ptr<const Element>& e) const
{
  ConstRelationPtr rr = boost::dynamic_pointer_cast<const Relation>(re);
  ConstRelationPtr r = boost::dynamic_pointer_cast<const Relation>(e);

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
