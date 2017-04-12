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
#include "MapScoringStatusAndRefTagValidator.h"

// Hoot
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>

namespace hoot
{

MapScoringStatusAndRefTagValidator::MapScoringStatusAndRefTagValidator()
{
}

bool MapScoringStatusAndRefTagValidator::allTagsAreValid(const ConstOsmMapPtr& map)
{
  //if first map has any element with a hoot::status = Unknown1 tag and a tag with key = REF2,
  //then fail
  const long numFirstInputBadTags =
    (int)FilteredVisitor::getStat(
      new ChainCriterion(
        new StatusCriterion(Status::Unknown1),
        new TagKeyCriterion(MetadataTags::Ref2())),
      new ElementCountVisitor(),
      map);

  //if second map has any element with a hoot::status = Unknown2 tag and a tag with key = REF1,
  //then fail
  const long numSecondInputBadTags =
    (int)FilteredVisitor::getStat(
      new ChainCriterion(
        new StatusCriterion(Status::Unknown2),
        new TagKeyCriterion(MetadataTags::Ref1())),
      new ElementCountVisitor(),
      map);

  return (numFirstInputBadTags + numSecondInputBadTags) == 0;
}

}
