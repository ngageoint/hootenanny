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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __REPROJECT_TO_GEOGRAPHIC_OP_H__
#define __REPROJECT_TO_GEOGRAPHIC_OP_H__

// hoot
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * If not already reprojected, reprojects the given map to a custom planar projection. See
 * MapProjector::reprojectToPlanar
 *
 * This is compatible with fourpass.
 */
class ReprojectToGeographicOp : public OsmMapOperation
{
public:

  static QString className() { return "hoot::ReprojectToGeographicOp"; }

  ReprojectToGeographicOp() = default;
  ~ReprojectToGeographicOp() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  QString getDescription() const override
  { return "Reprojects to a geographic projection"; }
};

}

#endif // __REPROJECT_TO_GEOGRAPHIC_OP_H__
