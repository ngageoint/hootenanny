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

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonNameExtractor)

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
  QStringList targetNames = target->getTags().getNames();
  targetNames.append(target->getTags().getPseudoNames());
  QStringList candidateNames = candidate->getTags().getNames();
  candidateNames.append(candidate->getTags().getPseudoNames());

  if (ConfigOptions().getPoiPolygonUseAddressNameMatching())
  {
    //where one of the pair has no name but has a combination of address fields that match the
    //other's name
    if (OsmSchema::getInstance().isBuilding(target) && !target->getTags().contains("name") &&
        target->getTags().contains("addr:housenumber") &&
        target->getTags().contains("addr:street"))
    {
      const QString houseNumber = target->getTags().get("addr:housenumber").trimmed();
      const QString street = target->getTags().get("addr:street").trimmed();
      if (!houseNumber.isEmpty() && !street.isEmpty())
      {
        targetNames.append(houseNumber + " " + street);
      }
    }
    else if (OsmSchema::getInstance().isBuilding(candidate) && !candidate->getTags().contains("name") &&
             candidate->getTags().contains("addr:housenumber") &&
             candidate->getTags().contains("addr:street"))
    {
      const QString houseNumber = candidate->getTags().get("addr:housenumber").trimmed();
      const QString street = candidate->getTags().get("addr:street").trimmed();
      if (!houseNumber.isEmpty() && !street.isEmpty())
      {
        candidateNames.append(houseNumber + " " + street);
      }
    }

    //similar to above, but with the combined "address" field; two can probably be combined
    if (OsmSchema::getInstance().isPoi(*target) && !candidate->getTags().contains("name") &&
        target->getTags().contains("address") &&
        candidate->getTags().contains("addr:housenumber") &&
        candidate->getTags().contains("addr:street"))
    {
      const QString houseNumber = candidate->getTags().get("addr:housenumber").trimmed();
      const QString street = candidate->getTags().get("addr:street").trimmed();
      const QString address = target->getTags().get("address").trimmed();
      if (!houseNumber.isEmpty() && !street.isEmpty() && !address.isEmpty())
      {
        candidateNames.append(houseNumber + " " + street);
        targetNames.append(address);
      }
    }
    else if (OsmSchema::getInstance().isPoi(*candidate) && !target->getTags().contains("name") &&
        candidate->getTags().contains("address") &&
        target->getTags().contains("addr:housenumber") &&
        target->getTags().contains("addr:street"))
    {
      const QString houseNumber = target->getTags().get("addr:housenumber").trimmed();
      const QString street = target->getTags().get("addr:street").trimmed();
      const QString address = candidate->getTags().get("address").trimmed();
      if (!houseNumber.isEmpty() && !street.isEmpty() && !address.isEmpty())
      {
        targetNames.append(houseNumber + " " + street);
        candidateNames.append(address);
      }
    }
  }

  if (ConfigOptions().getPoiPolygonRemoveOperatorNameMatching())
  {
    //remove operator
    if (target->getTags().contains("operator"))
    {
      const QString op = target->getTags().get("operator");
      targetNames.removeAll(op);
    }
    if (candidate->getTags().contains("operator"))
    {
      const QString op = candidate->getTags().get("operator");
      candidateNames.removeAll(op);
    }
  }

  double score = -1;

  for (int i = 0; i < targetNames.size(); i++)
  {
    for (int j = 0; j < candidateNames.size(); j++)
    {
      //const QString targetName = targetNames[i];
      //const QString candidateName = candidateNames[j];
      double thisScore = -1.0;
      //if (!targetName.trimmed().isEmpty() && !candidateName.trimmed().isEmpty())
      //{
        thisScore = _d->compare(targetNames[i], candidateNames[j]);
      //}
      //else
      //{
        //thisScore = 0.0;
      //}
      score = max(thisScore, score);
    }
  }

  if (score == -1)
  {
    return nullValue();
  }

  return score;
}

}
