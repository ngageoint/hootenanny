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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTagEligiblePoiCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ImplicitTagEligiblePoiCriterion)

bool ImplicitTagEligiblePoiCriterion::isSatisfied(const boost::shared_ptr<const Element>& element) const
{
  return
    element->getElementType() == ElementType::Node && element->getTags().getNames().size() > 0 &&
    hasEligibleKvp(element->getTags());
}

QStringList ImplicitTagEligiblePoiCriterion::getEligibleKvps(const Tags& tags) const
{
  LOG_TRACE("Retrieving POI kvps...");

  QStringList poiKvps;
  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString kvp = tagItr.key() % "=" % tagItr.value();
    LOG_VART(kvp);
    LOG_VART(OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()));
    //if the tag is not generic and has anything to do with pois, we'll use it
    if (kvp != QLatin1String("poi=yes") &&
        OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()))
    {
      poiKvps.append(kvp);
    }
  }
  return poiKvps;
}

bool ImplicitTagEligiblePoiCriterion::hasEligibleKvp(const Tags& tags) const
{
  LOG_TRACE("Retrieving POI kvps...");

  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString kvp = tagItr.key() % "=" % tagItr.value();
    LOG_VART(kvp);
    LOG_VART(OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()));
    //if the tag is not generic and has anything to do with pois, we'll use it
    if (kvp != QLatin1String("poi=yes") &&
        OsmSchema::getInstance().getCategories(kvp).intersects(OsmSchemaCategory::poi()))
    {
      return true;
    }
  }
  return false;
}

}

