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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef _ELEMENTS_NODE_DATA_H_
#define _ELEMENTS_NODE_DATA_H_

#include "ElementData.h"

namespace hoot {

class NodeData : public ElementData
{
public:

  NodeData() {}

  NodeData(const NodeData& nd) : ElementData(nd.getId(), nd.getTags(), nd.getCircularError()),
    _x(nd._x), _y(nd._y) {}

  NodeData(long id, double x, double y) : ElementData(id), _x(x), _y(y) {}

  virtual ~NodeData() {}

  double getX() const { return _x; }

  double getY() const { return _y; }

  void init(long id, double x, double y) { _id = id; _x = x; _y = y;
    _changeset = 0; _version = 1; _timestamp = 0; }

  void init(long id, double x, double y, long changeset, long version, long timestamp) {
   _id = id; _x = x; _y = y; _changeset = changeset; _version = version; _timestamp = timestamp; }

  void setX(double x) { _x = x; }

  void setY(double y) { _y = y; }

  NodeData& operator=(const NodeData& nd);

protected:

  double _x, _y;

};

} // hoot

#endif
