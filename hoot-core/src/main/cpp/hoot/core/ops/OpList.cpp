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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OpList.h"

// hoot
#include <hoot/core/io/ObjectInputStream.h>
#include <hoot/core/io/ObjectOutputStream.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, OpList)

OpList::OpList()
{
}

void OpList::apply(boost::shared_ptr<OsmMap>& map)
{
  for (size_t i = 0; i < _ops.size(); i++)
  {
    LOG_INFO("Applying op.");
    _ops[i]->apply(map);
  }
}

void OpList::readObject(QDataStream& is)
{
  uint size;
  is >> size;
  LOG_INFO("Reading " << size << " objects.");
  ObjectInputStream ois(is);
  for (uint i = 0; i < size; i++)
  {
    LOG_INFO("Reading op.");
    OsmMapOperation* op = ois.readObject<OsmMapOperation>();
    _ops.push_back(boost::shared_ptr<OsmMapOperation>(op));
  }
}

void OpList::setBounds(const Envelope &bounds)
{
  for (size_t i = 0; i < _ops.size(); i++)
  {
    Boundable* b = dynamic_cast<Boundable*>(_ops[i].get());
    if (b != 0)
    {
      b->setBounds(bounds);
    }
  }
}

void OpList::writeObject(QDataStream& os) const
{
  os << (uint)_ops.size();
  ObjectOutputStream oos(os);
  for (size_t i = 0; i < _ops.size(); i++)
  {
    oos.writeObject(*_ops[i]);
  }
}

}
