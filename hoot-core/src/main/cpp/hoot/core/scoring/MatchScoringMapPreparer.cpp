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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchScoringMapPreparer.h"

// hoot
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/IsNodeFilter.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/AddUuidVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagVisitor.h>

namespace hoot
{

  class ConvertUuidToRefVisitor : public ElementOsmMapVisitor
  {
    public:

      virtual void visit(const boost::shared_ptr<Element>& e)
      {
        if (!e->getTags().contains(MetadataTags::Ref1()) && !e->getTags().contains(MetadataTags::Ref2()) &&
            e->getTags().contains("uuid"))
        {
          QString uuid = e->getTags()["uuid"];
          if (e->getStatus() == Status::Unknown1)
          {
            e->setTag(MetadataTags::Ref1(), uuid);
          }
          else if (e->getStatus() == Status::Unknown2)
          {
            e->setTag(MetadataTags::Ref2(), uuid);
          }
        }
      }
  };

MatchScoringMapPreparer::MatchScoringMapPreparer()
{

}

void MatchScoringMapPreparer::prepMap(OsmMapPtr map, const bool removeNodes)
{
  // if an element has a uuid, but no REF1/REF2 tag then create a REF tag with the uuid. The
  // 1/2 is determined by the unknown status.
  ConvertUuidToRefVisitor convertUuidToRef;
  map->visitRw(convertUuidToRef);

  // #5891 if the feature is marked as todo then there is no need to conflate & evaluate it.
  boost::shared_ptr<TagCriterion> isTodo(new TagCriterion(MetadataTags::Ref2(), "todo"));
  RemoveElementsVisitor remover(isTodo);
  remover.setRecursive(true);
  map->visitRw(remover);

  // add a uuid to all elements with a REF tag.
  TagKeyCriterion criterion(MetadataTags::Ref1(), MetadataTags::Ref2(), "REVIEW");
  AddUuidVisitor uuid("uuid");
  FilteredVisitor v(criterion, uuid);
  map->visitRw(v);

  if (removeNodes)
  {
    // remove all REF1/REF2 tags from the nodes.
    RemoveTagVisitor removeRef(MetadataTags::Ref1(), MetadataTags::Ref2());
    IsNodeFilter nodeFilter(Filter::KeepMatches);
    FilteredVisitor removeRefV(nodeFilter, removeRef);
    map->visitRw(removeRefV);
  }
}

}
