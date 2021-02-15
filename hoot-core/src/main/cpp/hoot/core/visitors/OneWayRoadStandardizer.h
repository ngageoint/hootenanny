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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ONE_WAY_ROAD_STANDARDIZER_H
#define ONE_WAY_ROAD_STANDARDIZER_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Replaces all reversed one way roads with regular one way roads
 */
class OneWayRoadStandardizer : public ElementOsmMapVisitor
{

public:

  static QString className() { return "hoot::OneWayRoadStandardizer"; }

  OneWayRoadStandardizer() = default;
  virtual ~OneWayRoadStandardizer() = default;

  virtual void visit(const std::shared_ptr<Element>& e);

  virtual QString getInitStatusMessage() const
  { return "Standardizing one way roads..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Standardized " + QString::number(_numAffected) + " one way roads."; }

  virtual QString getDescription() const
  { return "Replaces all reversed one way roads with regular one way roads"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }
};

}

#endif // ONE_WAY_ROAD_STANDARDIZER_H
