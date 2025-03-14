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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#ifndef OSMMAPCONSUMER_H
#define OSMMAPCONSUMER_H

#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/** Polymorphic base class used for collections and casting */
class OsmMapConsumer
{
public:

  OsmMapConsumer() = default;
  virtual ~OsmMapConsumer() = default;

  virtual void setOsmMap(OsmMap* map) = 0;
};

/** Base class that implements OsmMapConsumer used to derive consumer class implementations */
class OsmMapConsumerBase : public OsmMapConsumer
{
public:

  OsmMapConsumerBase() = default;
  ~OsmMapConsumerBase() override = default;

  void setOsmMap(OsmMap* map) override { _map = map->shared_from_this(); }

protected:

  OsmMapPtr _map;

};

}

#endif // OSMMAPCONSUMER_H
