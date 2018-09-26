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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TranslateStringDistance.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/language/translators/DictionaryTranslator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>
#include <QStringList>

namespace hoot
{

HOOT_FACTORY_REGISTER(StringDistance, TranslateStringDistance)

boost::shared_ptr<ToEnglishTranslator> TranslateStringDistance::_translator;

TranslateStringDistance::TranslateStringDistance(StringDistancePtr d):
  _d(d)
{
  setConfiguration(conf());
}

void TranslateStringDistance::setConfiguration(const Settings& conf)
{
  ConfigOptions c(conf);
  _tokenize = c.getTranslateStringDistanceTokenize();
}

double TranslateStringDistance::compare(const QString& s1, const QString& s2) const
{
  double bestScore = -1.0;
  QString best1, best2;

  boost::shared_ptr<DictionaryTranslator> dictTranslator =
    boost::dynamic_pointer_cast<DictionaryTranslator>(_translator);
  if (dictTranslator)
  {
    QStringList t1;
    QStringList t2;
    if (_tokenize)
    {
      t1 = dictTranslator->toEnglishAll(s1);
      t2 = dictTranslator->toEnglishAll(s2);
    }
    else
    {
      t1.append(s1);
      t2.append(s2);
      t1 = dictTranslator->toEnglishAll(t1);
      t2 = dictTranslator->toEnglishAll(t2);
    }
    LOG_VART(t1);
    LOG_VART(t2);

    for (int i = 0; i < t1.size(); i++)
    {
      for (int j = 0; j < t2.size(); j++)
      {
        const double s = _d->compare(t1[i], t2[j]);
        if (s > bestScore)
        {
          bestScore = s;
          best1 = t1[i];
          best2 = t2[j];
        }
      }
    }
  }
  else
  {
    const QStringList names1 = _getNamesToScore(s1);
    const QStringList names2 = _getNamesToScore(s2);
    LOG_VART(names1);
    LOG_VART(names2);
    for (int i = 0; i < names1.size(); i++)
    {
      const QString name1 = names1.at(i);
      for (int j = 0; j < names2.size(); j++)
      {
        const QString name2 = names2.at(i);
        const double s = _d->compare(name1, name2);
        if (s > bestScore)
        {
          bestScore = s;
          best1 = name1;
          best2 = name2;
        }
      }
    }
  }

  LOG_TRACE("Best translation: " << best1 << " and " << best2 << " best score: " << bestScore);

  return bestScore;
}

QStringList TranslateStringDistance::_getNamesToScore(const QString name) const
{
  QStringList names;
  if (!name.trimmed().isEmpty())
  {
    names.append(name.trimmed());
    const QString translatedName = _translator->translate(name.trimmed());
    if (!translatedName.isEmpty())
    {
      names.append(translatedName);
    }
  }
  return names;
}

}
