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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_MAP_SPLITTER
#define OSM_MAP_SPLITTER

//  Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>

//  Geos
#include <geos/geom/Envelope.h>

//  Std
#include <set>
#include <vector>

namespace hoot
{

typedef QMap<geos::geom::Envelope, OsmMapPtr> TileMap;

class OsmMapSplitter : public Configurable
{
public:
  OsmMapSplitter(const OsmMapPtr& map, const OsmMapPtr& tiles);

  virtual void setConfiguration(const Settings& conf) override;

  void apply();

  void writeMaps(const QString& filename);

private:

  void _placeElementInMap(const ElementPtr& element);

  OsmMapPtr _map;
  OsmMapPtr _tiles;

  std::vector<geos::geom::Envelope> _tileEnvelopes;
  std::vector<OsmMapPtr> _tileMaps;

  std::set<ElementId> _eidsCompleted;

  long _statusUpdateInterval;
};

}

#endif  //  OSM_MAP_SPLITTER
