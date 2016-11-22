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
#include "PoiPolygonNameScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonNameScoreExtractor)

PoiPolygonNameScoreExtractor::PoiPolygonNameScoreExtractor()
{
}

void PoiPolygonNameScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setNameScoreThreshold(config.getPoiPolygonNameScoreThreshold());
}

double PoiPolygonNameScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  const double nameScore =
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
   .extract(poi, poly);
  LOG_VART(nameScore);
  return nameScore;
}

bool PoiPolygonNameScoreExtractor::elementHasName(ConstElementPtr element)
{
  const QStringList names = element->getTags().getNames();
  for (int i = 0; i < names.size(); i++)
  {
    if (!element->getTags().get("name").trimmed().isEmpty())
    {
      return true;
    }
  }
  return false;
}

QString PoiPolygonNameScoreExtractor::getElementName(ConstElementPtr element)
{
  QString name = element->getTags().get("name").toLower().trimmed();
  if (!name.isEmpty())
  {
    return name;
  }
  else
  {
    QStringList names = element->getTags().getNames();
    for (int i = 0; i < names.size(); i++)
    {
      name = names.at(i).toLower().trimmed();
      //arbitrarily returning the first name here
      if (!name.isEmpty())
      {
        return name;
      }
    }
  }
  return "";
}

}
