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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Node.h"

// Boost
using namespace boost;

namespace hoot
{

NodeData::NodeData(long id, double x, double y) :
ElementData(id),
_x(x),
_y(y)
{
}

NodeData::NodeData(const NodeData& from) :
ElementData(from.getId(), from.getTags(), from.getCircularError()),
_x(from._x),
_y(from._y)
{
  init(from.getId(), from.getX(), from.getY(), from.getChangeset(), from.getVersion(),
       from.getTimestamp(), from.getUser(), from.getUid(), from.getVisible());
}

NodeData& NodeData::operator=(const NodeData& nd)
{
  init(
    nd.getId(), nd.getX(), nd.getY(), nd.getChangeset(), nd.getVersion(), nd.getTimestamp(),
    nd.getUser(), nd.getUid(), nd.getVisible());
  setTags(nd.getTags());
  setCircularError(nd.getCircularError());

  return *this;
}

void NodeData::init(long id, double x, double y, long changeset, long version, long timestamp,
                    QString user, long uid, bool visible)
{
 _id = id;
 _x = x;
 _y = y;
 _changeset = changeset;
 _version = version;
 _timestamp = timestamp;
 _user = user;
 _uid = uid;
 _visible = visible;
}

}
