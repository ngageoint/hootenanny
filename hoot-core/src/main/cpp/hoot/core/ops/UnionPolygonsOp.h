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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UNION_POLYGONS_OP_H
#define UNION_POLYGONS_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/UnionPolygonsVisitor.h>

namespace hoot
{

class OsmMap;

/**
 * A map operation making use of UnionPolyonsVisitor
 */
class UnionPolygonsOp : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::UnionPolygonsOp"; }

  UnionPolygonsOp();

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual std::string getClassName() const { return className(); }

  virtual QString getDescription() const override { return _combiner->getDescription(); }

  virtual QString getInitStatusMessage() const { return _combiner->getInitStatusMessage(); }

  virtual QString getCompletedStatusMessage() const
  { return _combiner->getCompletedStatusMessage(); }

private:

  std::shared_ptr<UnionPolygonsVisitor> _combiner;
};

}

#endif // UNION_POLYGONS_OP_H
