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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchScoringMapPreparer.h"

// hoot
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/AddUuidVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveTagsVisitor.h>

namespace hoot
{

class ConvertUuidToRefVisitor : public ElementOsmMapVisitor
{
public:

  virtual void visit(const std::shared_ptr<Element>& e)
  {
    if (!e->getTags().contains(MetadataTags::Ref1()) &&
        !e->getTags().contains(MetadataTags::Ref2()) && e->getTags().contains("uuid"))
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

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }
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
  std::shared_ptr<TagCriterion> isTodo(new TagCriterion(MetadataTags::Ref2(), "todo"));
  RemoveElementsVisitor remover;
  remover.setRecursive(true);
  remover.addCriterion(isTodo);
  map->visitRw(remover);

  // add a uuid to all elements with a REF tag.
  TagKeyCriterion criterion(MetadataTags::Ref1(), MetadataTags::Ref2(), MetadataTags::Review());
  AddUuidVisitor uuid("uuid");
  FilteredVisitor v(criterion, uuid);
  map->visitRw(v);

  if (removeNodes)
  {
    // remove all REF1/REF2 tags from the nodes.
    QStringList keys;
    keys.append(MetadataTags::Ref1());
    keys.append(MetadataTags::Ref2());
    RemoveTagsVisitor removeRef(keys);
    NodeCriterion nodeCrit;
    FilteredVisitor removeRefV(nodeCrit, removeRef);
    map->visitRw(removeRefV);
  }
}

}
