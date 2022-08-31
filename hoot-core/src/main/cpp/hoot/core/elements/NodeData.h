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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef _ELEMENTS_NODE_DATA_H_
#define _ELEMENTS_NODE_DATA_H_

#include <hoot/core/elements/ElementData.h>

namespace hoot
{

class NodeData : public ElementData
{
public:

  NodeData() = default;
  NodeData(const NodeData& nd);
  NodeData(long id, double x, double y);
  ~NodeData() override = default;

  void init(long id, double x, double y, long changeset = ElementData::CHANGESET_EMPTY,
            long version = ElementData::VERSION_EMPTY,
            quint64 timestamp = ElementData::TIMESTAMP_EMPTY,
            QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
            bool visible = ElementData::VISIBLE_EMPTY);

  double getX() const { return _x; }
  double getY() const { return _y; }

  void setX(double x) { _x = x; }
  void setY(double y) { _y = y; }

  NodeData& operator=(const NodeData& nd);

private:

  double _x, _y;
};

inline void NodeData::init(long id, double x, double y, long changeset, long version,
                           quint64 timestamp, QString user, long uid, bool visible)
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

#endif
