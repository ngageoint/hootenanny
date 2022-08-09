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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef FIND_STREET_INTERSECTIONS_BY_NAME_H
#define FIND_STREET_INTERSECTIONS_BY_NAME_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/NameCriterion.h>
#include <hoot/core/info/OperationStatus.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Allows for finding the intersection of two streets by name and writing the intersection node
 * out to a map. The output nodes are labeled with the names of the intersecting streets found.
 */
class FindStreetIntersectionsByName : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "FindStreetIntersectionsByName"; }

  FindStreetIntersectionsByName() = default;
  ~FindStreetIntersectionsByName() override = default;

  void apply(OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Locates street intersections by street name"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see OperationStatus
   */
  QString getInitStatusMessage() const override { return "Locating street intersections..."; }
  /**
   * @see OperationStatus
   */
  QString getCompletedStatusMessage() const override;

private:

  std::shared_ptr<NameCriterion> _nameCrit;

  OsmMapPtr _filterRoadsByStreetName(
    const QString& name, const Status& status, const ConstOsmMapPtr& map) const;
};

}

#endif // FIND_STREET_INTERSECTIONS_BY_NAME_H
