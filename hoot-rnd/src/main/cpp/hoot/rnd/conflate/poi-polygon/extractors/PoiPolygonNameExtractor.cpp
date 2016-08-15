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
#include "PoiPolygonNameExtractor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

//HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonNameExtractor)

PoiPolygonNameExtractor::PoiPolygonNameExtractor()
{
  _d.reset(new LevenshteinDistance());
}

PoiPolygonNameExtractor::PoiPolygonNameExtractor(StringDistance* d)
{
  _d.reset(d);
}

double PoiPolygonNameExtractor::extract(const ConstElementPtr& target,
                                        const ConstElementPtr& candidate) const
{
  double score = -1.0;

  const QString targetName = target->getTags().get("name");
  const QString candidateName = candidate->getTags().get("name");

  QStringList targetNames = target->getTags().getNames();
  targetNames.append(target->getTags().getPseudoNames());
  QStringList candidateNames = candidate->getTags().getNames();
  candidateNames.append(candidate->getTags().getPseudoNames());

  //custom rules
  if (ConfigOptions().getPoiPolygonUseAbbreviationSubstitutions())
  {
    if (targetName.toLower().trimmed().contains("rec center") &&
        candidateName.toLower().trimmed().contains("recreation center"))
    {
      QString targetNameMod = targetName;
      targetNameMod.replace("Rec Center", "Recreation Center", Qt::CaseInsensitive);
      targetNames.append(targetNameMod);
    }
    if (candidateName.toLower().trimmed().contains("rec center") &&
        targetName.toLower().trimmed().contains("recreation center"))
    {
      QString candidateNameMod = candidateName;
      candidateNameMod.replace("Rec Center", "Recreation Center", Qt::CaseInsensitive);
      candidateNames.append(candidateNameMod);
    }

    if (targetName.toLower().trimmed().contains("bldg"))
    {
      QString targetNameMod = targetName;
      targetNameMod.replace("Bldg", "Building", Qt::CaseInsensitive);
      targetNames.append(targetNameMod);
    }
    if (candidateName.toLower().trimmed().contains("bldg"))
    {
      QString candidateNameMod = candidateName;
      candidateNameMod.replace("Bldg", "Building", Qt::CaseInsensitive);
      candidateNames.append(candidateNameMod);
    }
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
