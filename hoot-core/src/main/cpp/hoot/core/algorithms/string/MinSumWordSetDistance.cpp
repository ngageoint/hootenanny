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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MinSumWordSetDistance.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/schema/ScoreMatrix.h>

// Qt
#include <QStringList>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, MinSumWordSetDistance)

MinSumWordSetDistance::MinSumWordSetDistance(StringDistance* d)
{
  _d.reset(d);
}

MinSumWordSetDistance::MinSumWordSetDistance()
{
}

double MinSumWordSetDistance::compare(const QString& s1, const QString& s2) const
{
  QStringList sl1 = s1.split(" ");
  QStringList sl2 = s2.split(" ");

  ScoreMatrix m(sl1.size(), sl2.size());

  for (int i = 0; i < sl1.size(); i++)
  {
    for (int j = 0; j < sl2.size(); j++)
    {
      // value from 0 (similar) to 1 (dissimilar)
      double v = 1 - _d->compare(sl1[i], sl2[j]);
      m.set(i, j, v);
    }
  }

  return m.minSumScore();
}

}
