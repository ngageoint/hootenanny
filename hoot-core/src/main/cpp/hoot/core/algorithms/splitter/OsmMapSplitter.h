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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef OSM_MAP_SPLITTER
#define OSM_MAP_SPLITTER

//  Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>

//  Geos
#include <geos/geom/Envelope.h>

//  Std



namespace hoot
{

/**
 * @brief The OsmMapSplitter class splits a map up into multiple maps based on preprocessed tile
 * boundaries.  Elements that span tiles will only be assigned to one tile output map so that they
 * aren't duplicated in conflation campaigns.
 */
class OsmMapSplitter : public Configurable
{
public:

  OsmMapSplitter(const OsmMapPtr& map, const OsmMapPtr& tiles);
  ~OsmMapSplitter() = default;

  void setConfiguration(const Settings& conf) override;

  void apply();
  /**
   * @brief writeMaps Write all maps to disk with only their corresponding elements
   * @param filename Base filename for all files in the format:
   *   output-00001.osm
   *   ...
   *   output-0000n.osm
   */
  void writeMaps(const QString& filename);

private:

  /**
   * @brief _placeElementInMap Take an element from the original map and find the tile map that
   *  that it falls into and then copy it to the new map
   * @param element Element to copy into tile map
   */
  void _placeElementInMap(const ElementPtr& element);

  using TileMap = QMap<geos::geom::Envelope, OsmMapPtr>;
  /** Original map to split up */
  OsmMapPtr _map;
  /** Map of polygon ways that indicate tile divisions */
  OsmMapPtr _tiles;
  /** Vector of tile envelopes to check new elements against */
  std::vector<geos::geom::Envelope> _tileEnvelopes;
  /** Vector of maps whose index corresponds to the tile envelopes above */
  std::vector<OsmMapPtr> _tileMaps;
  /** Set of all elements that have already been processed so that no elements are repeated */
  std::set<ElementId> _eidsCompleted;

  long _statusUpdateInterval;
};

}

#endif  //  OSM_MAP_SPLITTER
