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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef REMOVE_WAYS_BY_BOUNDS_OP_H
#define REMOVE_WAYS_BY_BOUNDS_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/util/Configurable.h>

// GEOS
#include <geos/geom/Envelope.h>

// Standard
#include <set>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

/**
 * Remove all ways either inside or outside the given bounds.
 */
class RemoveWaysByBoundsOp : public OsmMapOperation, public OperationStatusInfo,
  public Configurable
{
public:

  static std::string className() { return "hoot::RemoveWaysByBoundsOp"; }

  RemoveWaysByBoundsOp();
  RemoveWaysByBoundsOp(const geos::geom::Envelope& e, const bool inverse = false);

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual QString getInitStatusMessage() const;
  virtual QString getCompletedStatusMessage() const;

  virtual QString getDescription() const
  { return "Removes ways in relation to a specified bounds"; }

  virtual void setConfiguration(const Settings& conf);

  void setBounds(const geos::geom::Envelope& bounds) { _envelope = bounds; }
  void setInvertBounds(const bool invert) { _inverse = invert; }

protected:

  geos::geom::Envelope _envelope;
  bool _inverse;
  boost::shared_ptr<OsmMap> _map;

  void _removeWays();
};

}

#endif // REMOVE_WAYS_BY_BOUNDS_OP_H
