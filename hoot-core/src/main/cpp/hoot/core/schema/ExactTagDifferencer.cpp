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
 * @copyright Copyright (C) 2015, 2017, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ExactTagDifferencer.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QSet>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagDifferencer, ExactTagDifferencer)

double ExactTagDifferencer::diff(const ConstOsmMapPtr& /*map*/, const ConstElementPtr& e1,
  const ConstElementPtr& e2) const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  QSet<QString> touched;

  for (Tags::const_iterator it = t1.begin(); it != t1.end(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    touched.insert(it.key());

    LOG_VART(OsmSchema::getInstance().isMetaData(it.key(), it.value()));
    if (OsmSchema::getInstance().isMetaData(it.key(), it.value()) == false)
    {
      if (it.value() != t2.get(it.key()))
      {
        LOG_TRACE(
          "Returning exact tag difference on key: " << it.key() << " for " << e1->getElementId() <<
          " and " << e2->getElementId() << "...");
        return 1;
      }
    }
  }
  LOG_VART(touched);

  for (Tags::const_iterator it = t2.begin(); it != t2.end(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());

    if (touched.contains(it.key()) == false)
    {
      LOG_VART(OsmSchema::getInstance().isMetaData(it.key(), it.value()));
      if (OsmSchema::getInstance().isMetaData(it.key(), it.value()) == false)
      {
        if (it.value() != t1.get(it.key()))
        {
          LOG_TRACE(
            "Returning exact tag difference on key: " << it.key() << " for " <<
            e1->getElementId() << " and " << e2->getElementId() << "...");
          return 1;
        }
      }
    }
  }

  LOG_TRACE(
    "Returning exact tag match for " << e1->getElementId() << " and " << e2->getElementId() <<
    "...");
  return 0;

  // TODO: Possibly, the above could be replaced with this below but still need to sort through test
  // failures caused by the change.
//  if (e1->getTags().hasSameNonMetadataTags(e2->getTags()))
//  {
//    return 0.0;
//  }
//  return 1.0;
}

}
