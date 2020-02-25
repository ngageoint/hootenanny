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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef _POINTS_TO_POLYS_CONVERTER_H_
#define _POINTS_TO_POLYS_CONVERTER_H_

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Converts any node in a map that isn't a child of another element to a small square polygon. This
 * can be useful if you need to calculate an alpha shape for a small number of points.
 */
class PointsToPolysConverter : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::PointsToPolysConverter"; }

  PointsToPolysConverter(const double bufferSize = 1.0);

  virtual void apply(OsmMapPtr& map);

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const
  { return "Converting non-child points to polygons..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Converted " + StringUtils::formatLargeNumber(_numAffected) + " / " +
      StringUtils::formatLargeNumber(_numProcessed) + " non-child points to polygons."; }

  virtual QString getDescription() const
  { return "Converts non-child points to rectangular polygons"; }

private:

  double _bufferSize;
  QList<long> _nodeIdsConverted;

  void _addPolyFromPoint(const ConstNodePtr& point, const OsmMapPtr& map);
};

}

#endif // _POINTS_TO_POLYS_CONVERTER_H_
