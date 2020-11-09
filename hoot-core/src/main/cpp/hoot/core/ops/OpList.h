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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OPLIST_H
#define OPLIST_H

// hoot
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Contains a list of OsmMapOperations that will be applied in order. It is possible to create
 * more complex structures containing tress of OpLists, but there may be limited utility. Creating
 * self-referencing or loops of OpLists will result in undefined behaviour. E.g.
 *
 * // bad!
 * std::shared_ptr<OpList> a(new OpList());
 * a->addOp(a);
 */
class OpList : public OsmMapOperation, public Boundable
{
public:

  static std::string className() { return "hoot::OpList"; }

  OpList() = default;
  virtual ~OpList() = default;

  void addOp(const std::shared_ptr<OsmMapOperation>& op) { _ops.push_back(op); }

  /**
   * Applies the operations in the order they were added. If there are no operations then nothing
   * is done.
   */
  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual std::string getClassName() const { return className(); }

  virtual void setBounds(const std::shared_ptr<geos::geom::Geometry>& bounds);

  virtual QString getDescription() const { return ""; }

private:

  std::vector<std::shared_ptr<OsmMapOperation>> _ops;
};

}

#endif // OPLIST_H
