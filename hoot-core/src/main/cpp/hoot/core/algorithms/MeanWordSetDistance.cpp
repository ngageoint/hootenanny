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
#include "MeanWordSetDistance.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/ScoreMatrix.h>

// Qt
#include <QStringList>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, MeanWordSetDistance)

MeanWordSetDistance::MeanWordSetDistance(StringDistance* d, double portion)
{
  _d.reset(d);
  _p = portion;
}

MeanWordSetDistance::MeanWordSetDistance()
{
  _p = 1.0;
}

double MeanWordSetDistance::compare(const QString& s1, const QString& s2) const
{
  QStringList sl1 = _tokenizer.tokenize(s1);
  QStringList sl2 = _tokenizer.tokenize(s2);

  ScoreMatrix m(sl1.size(), sl2.size());

  for (int i = 0; i < sl1.size(); i++)
  {
    for (int j = 0; j < sl2.size(); j++)
    {
      m.set(i, j, _d->compare(sl1[i], sl2[j]));
    }
  }

  double score = 0;

  if (sl1.size() > 0 && sl2.size() > 0)
  {
    score = m.meanScore(_p);
  }

  return score;
}

}
