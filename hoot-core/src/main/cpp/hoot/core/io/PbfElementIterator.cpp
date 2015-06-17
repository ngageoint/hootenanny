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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PbfElementIterator.h"

// Standard
#include <fstream>

namespace hoot
{

PbfElementIterator::PbfElementIterator(QString path)
{
  fstream* fp = new fstream();
  fp->open(path.toUtf8().data(), ios::in | ios::binary);
  if (fp->is_open() == false)
  {
    throw HootException("Error opening " + path + " for writing.");
  }
  _init(fp);
}

PbfElementIterator::PbfElementIterator(istream* in)
{
  _init(in);
}

void PbfElementIterator::_init(istream* in)
{
  _in.reset(in);
  _reader.reset(new PbfReader(false));
  _reader->setPermissive(true);
  _reader->setUseDataSourceIds(true);
  _reader->setUseFileStatus(false);
  _blobs = _reader->loadOsmDataBlobOffsets(*in);
  _map.reset(new OsmMap());
  _blobIndex = 0;
}

void PbfElementIterator::_next()
{
  if (_blobIndex < (int)_blobs.size())
  {
    _reader->parseBlob(_blobs[_blobIndex++], _in.get(), _map);

    const OsmMap::NodeMap& nodes = _map->getNodeMap();
    for (OsmMap::NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
      _addElement(_map->getNode(it.key()));
    }

    const WayMap& ways = _map->getWays();
    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
    {
      _addElement(_map->getWay(it->first));
    }

    const RelationMap& relations = _map->getRelationMap();
    for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
    {
      _addElement(_map->getRelation(it->first));
    }

    _map->clear();
  }
}

}
