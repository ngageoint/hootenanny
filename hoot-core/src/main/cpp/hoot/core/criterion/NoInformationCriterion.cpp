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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "NoInformationCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/Tags.h>

#include <hoot/core/elements/Element.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NoInformationCriterion)

NoInformationCriterion::NoInformationCriterion()
{
  setConfiguration(conf());
}

NoInformationCriterion::NoInformationCriterion(bool treatReviewTagsAsMetadata) :
_treatReviewTagsAsMetadata(treatReviewTagsAsMetadata)
{
}

bool NoInformationCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const Tags tags = e->getTags();
  const int informationCount = tags.getInformationCount();
  const int reviewTagCount =
    tags.getList("regex?" + MetadataTags::HootReviewTagPrefix() + ".*").size();

  //LOG_VART(e);
  LOG_VART(e->getElementId());
  LOG_VART(informationCount);
  LOG_VART(_treatReviewTagsAsMetadata);
  LOG_VART(reviewTagCount);

  bool isSatisified = informationCount == 0;
  if (!_treatReviewTagsAsMetadata)
  {
    isSatisified &= reviewTagCount == 0;
  }
  LOG_VART(isSatisified);
  return isSatisified;
}

void NoInformationCriterion::setConfiguration(const Settings& conf)
{
  _treatReviewTagsAsMetadata = ConfigOptions(conf).getWriterCleanReviewTags();
}

}

