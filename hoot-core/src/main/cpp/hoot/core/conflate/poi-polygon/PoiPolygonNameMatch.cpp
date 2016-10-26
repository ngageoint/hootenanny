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
#include "PoiPolygonNameMatch.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/algorithms/ExactStringDistance.h>

namespace hoot
{

PoiPolygonNameMatch::PoiPolygonNameMatch(double nameScoreThreshold) :
_nameScoreThreshold(nameScoreThreshold)
{

}

double PoiPolygonNameMatch::getNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha()))))
   .extract(e1, e2);
}

double PoiPolygonNameMatch::getExactNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  const QString e1Name = e1->getTags().get("name");
  const QString e2Name = e2->getTags().get("name");
  if (e1Name.trimmed().isEmpty() && e2Name.trimmed().isEmpty())
  {
    return 0.0;
  }
  //TODO: fix
  /*return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(
          new ExactStringDistance())))
   .extract(e1, e2);*/
   return ExactStringDistance().compare(e1Name.toLower(), e2Name.toLower());
}

//TODO: make work for all name tag types
bool PoiPolygonNameMatch::elementHasName(ConstElementPtr element)
{
  return !element->getTags().get("name").trimmed().isEmpty();
}

}
