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
#include "TranslateStringDistance.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>
#include <QStringList>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, TranslateStringDistance)

TranslateStringDistance::TranslateStringDistance(StringDistance *d)
{
  _d.reset(d);
  setConfiguration(conf());
}

double TranslateStringDistance::compare(const QString& s1, const QString& s2) const
{
  QStringList t1;
  QStringList t2;
  if (_tokenize)
  {
    t1 = Translator::getInstance().toEnglishAll(s1);
    t2 = Translator::getInstance().toEnglishAll(s2);
  }
  else
  {
    t1.append(s1);
    t2.append(s2);
    t1 = Translator::getInstance().toEnglishAll(t1);
    t2 = Translator::getInstance().toEnglishAll(t2);
  }

  double bestScore = -1;
  QString best1, best2;
  for (int i = 0; i < t1.size(); i++)
  {
    for (int j = 0; j < t2.size(); j++)
    {
      double s = _d->compare(t1[i], t2[j]);
      if (s > bestScore)
      {
        bestScore = s;
        best1 = t1[i];
        best2 = t2[j];
      }
    }
  }

//  LOG_INFO("Best translation: " << best1 << " and " << best2 <<
//           " best score: " << bestScore);

  return bestScore;
}

void TranslateStringDistance::setConfiguration(const Settings& conf)
{
  ConfigOptions c(conf);
  _tokenize = c.getTranslateStringDistanceTokenize();
}

}
