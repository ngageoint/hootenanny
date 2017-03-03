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

double ExactTagDifferencer::diff(const ConstOsmMapPtr &/*map*/, const ConstElementPtr& e1,
  const ConstElementPtr &e2) const
{
  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  QSet<QString> touched;

  for (Tags::const_iterator it = t1.begin(); it != t1.end(); ++it)
  {
    touched.insert(it.key());

    if (OsmSchema::getInstance().isMetaData(it.key(), it.value()) == false)
    {
      if (it.value() != t2.get(it.key()))
      {
        return 1;
      }
    }
  }

  for (Tags::const_iterator it = t2.begin(); it != t2.end(); ++it)
  {
    if (touched.contains(it.key()) == false)
    {
      if (OsmSchema::getInstance().isMetaData(it.key(), it.value()) == false)
      {
        if (it.value() != t2.get(it.key()))
        {
          return 1;
        }
      }
    }
  }

  return 0;
}

}
