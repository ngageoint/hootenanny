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
 * @copyright Copyright (C) 2016, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CONSTOSMMAPCONSUMER_H
#define CONSTOSMMAPCONSUMER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{
class OsmMap;

class ConstOsmMapConsumer : public OsmMapConsumer
{
public:

  ConstOsmMapConsumer() = default;
  virtual ~ConstOsmMapConsumer() = default;

  virtual void setOsmMap(const OsmMap* map) = 0;

  void setOsmMap(OsmMap* map) override
  {
    setOsmMap((const OsmMap*)map);
  }
};

class ConstOsmMapConsumerImpl : public ConstOsmMapConsumer
{
public:
  ConstOsmMapConsumerImpl() = default;
  ConstOsmMapConsumerImpl(const ConstOsmMapPtr& map) : _map(map) { }
  ~ConstOsmMapConsumerImpl() override = default;

  virtual void setOsmMap(const OsmMap* map) override { _map = map->shared_from_this(); }

protected:
  ConstOsmMapPtr _map;
};

}

#endif // CONSTOSMMAPCONSUMER_H
