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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WAYBUFFERCRITERION_H
#define WAYBUFFERCRITERION_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

class WayBufferCriterion : public ElementCriterion
{
public:

  static QString className() { return "WayBufferCriterion"; }

  WayBufferCriterion() = default;
  ~WayBufferCriterion() override = default;
  /**
   * Buffer is the buffer in meters to put around the way. The circular
   * error of the base way and the way being evaluated will automatically
   * be added to to the buffer on evaluation.
   */
  WayBufferCriterion(ConstOsmMapPtr map, ConstWayPtr baseWay, Meters buffer, double matchPercent);
  /**
   * Buffer is the buffer in meters to put around the way. The circular
   * error of the the way being evaluated will automatically be added
   * to to the buffer on evaluation.
   * @param circularError circular error of baseLine
   */
  WayBufferCriterion(
    ConstOsmMapPtr map, std::shared_ptr<geos::geom::LineString> baseLine, Meters buffer,
    Meters circularError, double matchPercent);

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<WayBufferCriterion>(_map, _baseLs, _buffer, 0, _matchPercent); }

  QString getDescription() const override { return "Allows for operations on ways with buffers"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  Meters _buffer;
  mutable std::shared_ptr<geos::geom::Geometry> _baseBuffered;
  std::shared_ptr<geos::geom::LineString> _baseLs;

  // Anything outside the given bounds cannot be within the max distance.
  mutable geos::geom::Envelope _boundsPlus;
  mutable Meters _baseLength;
  mutable Meters _bufferAccuracy;
  ConstOsmMapPtr _map;
  double _matchPercent;
};

}

#endif // WAYBUFFERCRITERION_H
