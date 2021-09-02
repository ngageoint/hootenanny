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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HIGHWAYCLASSIFIER_H
#define HIGHWAYCLASSIFIER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/conflate/matching/MatchClassification.h>

namespace hoot
{

class HighwayClassifier
{
public:

  static QString className() { return "HighwayClassifier"; }

  HighwayClassifier() = default;
  virtual ~HighwayClassifier() = default;

  /**
   * Classifies the match type of a subline match and returns the results.
   */
  virtual MatchClassification classify(
    const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2,
    const WaySublineMatchString& match) = 0;

  /**
   * See note in MatchDetails::getFeatures about the return type of this method.
   */
  virtual std::map<QString, double> getFeatures(
    const ConstOsmMapPtr& m, ElementId eid1, ElementId eid2,
    const WaySublineMatchString& match) const = 0;
};

}

#endif // HIGHWAYCLASSIFIER_H
