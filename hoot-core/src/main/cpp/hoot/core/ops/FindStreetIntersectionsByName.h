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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FIND_STREET_INTERSECTIONS_BY_NAME_H
#define FIND_STREET_INTERSECTIONS_BY_NAME_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/NameCriterion.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class OsmMap;

/**
 * Allows for finding the intersection of two streets by name and writing the intersection node
 * out to a map. The output nodes are labeled with the names of the intersecting streets found.
 */
class FindStreetIntersectionsByName : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::FindStreetIntersectionsByName"; }

  FindStreetIntersectionsByName() = default;
  virtual ~FindStreetIntersectionsByName() = default;

  virtual void apply(OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const override
  { return "Locates street intersections by street name"; }

  virtual std::string getClassName() const { return className(); }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getInitStatusMessage() const { return "Locating street intersections..."; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getCompletedStatusMessage() const
  {
    return
      "Located " + StringUtils::formatLargeNumber(_numAffected) +
      " street intersections for inputs: " + _nameCrit->getNames()[0] + " and " +
      _nameCrit->getNames()[1] + " out of " + StringUtils::formatLargeNumber(_numProcessed) +
      " streets.";
  }

private:

  std::shared_ptr<NameCriterion> _nameCrit;
};

}

#endif // FIND_STREET_INTERSECTIONS_BY_NAME_H
