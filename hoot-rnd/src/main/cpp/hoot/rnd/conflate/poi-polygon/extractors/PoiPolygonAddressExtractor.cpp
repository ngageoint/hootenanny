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
#include "PoiPolygonAddressExtractor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressExtractor)

PoiPolygonAddressExtractor::PoiPolygonAddressExtractor()
{
  _d.reset(new LevenshteinDistance());
}

PoiPolygonAddressExtractor::PoiPolygonAddressExtractor(StringDistance* d)
{
  _d.reset(d);
}

double PoiPolygonAddressExtractor::extract(const ConstElementPtr& target,
                                           const ConstElementPtr& candidate) const
{
  double score = -1.0;

  QStringList targetNames;
  QStringList candidateNames;
  Tags e1Tags = target->getTags();
  Tags e2Tags = candidate->getTags();

  //TODO: hack - this should be able to be eliminated by using the translated name comparison
  //logic
  QChar eszett(0x00DF);

  const QString e1HouseNum = e1Tags.get("addr:housenumber").trimmed();
  QString e1Street = e1Tags.get("addr:street").trimmed().toLower();
  //if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  //{
    e1Street = e1Street.replace(eszett, "ss");
  //}
  QString e1AddrComb;
  if (!e1HouseNum.isEmpty() && !e1Street.isEmpty())
  {
    e1AddrComb = e1HouseNum + " " + e1Street;
  }
  QString e1AddrTag = e1Tags.get("address").trimmed().toLower();
  //if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  //{
    e1AddrTag = e1AddrTag.replace(eszett, "ss");
  //}
  const QString e2HouseNum = e2Tags.get("addr:housenumber").trimmed();
  QString e2Street = e2Tags.get("addr:street").trimmed().toLower();
  //if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  //{
    e2Street = e2Street.replace(eszett, "ss");
  //}
  QString e2AddrComb;
  if (!e2HouseNum.isEmpty() && !e2Street.isEmpty())
  {
    e2AddrComb = e2HouseNum + " " + e2Street;
  }
  QString e2AddrTag = e2Tags.get("address").trimmed().toLower();
  //if (ConfigOptions().getPoiPolygonUseAddressTranslationRules())
  //{
    e2AddrTag = e2AddrTag.replace(eszett, "ss");
  //}

  if (!e1AddrComb.isEmpty() && !e2AddrComb.isEmpty())
  {
    targetNames.append(e1AddrComb);
    candidateNames.append(e2AddrComb);
  }
  else if (!e1AddrComb.isEmpty() && !e2AddrTag.isEmpty())
  {
    targetNames.append(e1AddrComb);
    candidateNames.append(e2AddrTag);
  }
  else if (!e2AddrComb.isEmpty() && !e1AddrTag.isEmpty())
  {
    targetNames.append(e1AddrTag);
    candidateNames.append(e2AddrComb);
  }
  else if (!e1AddrTag.isEmpty() && !e2AddrTag.isEmpty())
  {
    targetNames.append(e1AddrTag);
    candidateNames.append(e2AddrTag);
  }

  if (!targetNames.isEmpty() && !candidateNames.isEmpty())
  {
    for (int i = 0; i < targetNames.size(); i++)
    {
      for (int j = 0; j < candidateNames.size(); j++)
      {
        double thisScore = _d->compare(targetNames[i], candidateNames[j]);
        score = max(thisScore, score);
      }
    }
  }

  if (score == -1)
  {
    return nullValue();
  }

  return score;
}

}
