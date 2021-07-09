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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "OsmMapSplitter.h"

//  Hoot
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/StringUtils.h>

//  Qt
#include <QElapsedTimer>
#include <QFileInfo>

using geos::geom::Envelope;
using std::vector;

namespace hoot
{

OsmMapSplitter::OsmMapSplitter(const OsmMapPtr &map, const OsmMapPtr &tiles)
  : _map(map),
    _tiles(tiles),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10)
{
}

void OsmMapSplitter::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _statusUpdateInterval = configOptions.getTaskStatusUpdateInterval() * 10;
}

void OsmMapSplitter::apply()
{
  QElapsedTimer timer;
  timer.start();
  //  First build up the tile map
  const WayMap& tiles = _tiles->getWays();
  for (WayMap::const_iterator tile_it = tiles.begin(); tile_it != tiles.end(); ++tile_it)
  {
    _tileEnvelopes.push_back(tile_it->second->getEnvelopeInternal(_tiles));
    _tileMaps.push_back(std::make_shared<OsmMap>(_map->getProjection()));
  }
  //  Iterate all of the elements in the map and copy them to the corresponding map by envelope
  //  Start with relations
  const RelationMap& relations = _map->getRelations();
  for (RelationMap::const_iterator relation_it = relations.begin(); relation_it != relations.end(); ++relation_it)
    _placeElementInMap(relation_it->second);

  //  Then ways that haven't already been copied
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator way_it = ways.begin(); way_it != ways.end(); ++way_it)
    _placeElementInMap(way_it->second);

  //  Finally nodes that haven't already been copied
  const NodeMap& nodes = _map->getNodes();
  for (NodeMap::const_iterator node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
    _placeElementInMap(node_it->second);

  LOG_INFO(
    "Sorted " << StringUtils::formatLargeNumber(_eidsCompleted.size()) <<
    " elements in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

void OsmMapSplitter::_placeElementInMap(const ElementPtr& element)
{
  //  Check if the element has already been assigned a tile
  if (_eidsCompleted.find(element->getElementId()) != _eidsCompleted.end())
    return;
  //  Get the envelope of the element and start comparing it
  const Envelope& env = element->getEnvelopeInternal(_map);
  if (env.isNull())
  {
    LOG_WARN("Element " << element->getElementId() << " returned NULL envelope, ignoring.");
    return;
  }
  Envelope maxIntersectEnvelope;
  long maxEnvelopeIndex = -1;
  for (long tile_index = 0; tile_index < (long)_tileEnvelopes.size(); ++tile_index)
  {
    const Envelope& renv = _tileEnvelopes[tile_index];
    //  Find the best envelope for this element
    if (renv.contains(env))
    {
      //  This element is completely contained within the tile envelope, no further calculations needed
      maxEnvelopeIndex = tile_index;
      break;
    }
    else if (renv.intersects(env))
    {
      //  Partial intersection requires that we find the envelope that contains the highest percentage
      //  overlap, that can be done by finding the envelope with the highest area of intersection
      Envelope e;
      renv.intersection(env, e);
      if (maxIntersectEnvelope.isNull() || e.getArea() > maxIntersectEnvelope.getArea())
      {
        maxIntersectEnvelope = e;
        maxEnvelopeIndex = tile_index;
      }
    }
  }
  //  This should never happen, all elements should intersect with at least one envelope, if not more
  if (maxEnvelopeIndex < 0)
  {
    QString error = "Error: " + element->getElementId().toString() + " does not intersect any tile bounds.";
    LOG_ERROR(error);
    throw HootException(error);
  }
  //  Copy the relation (and all of its children) to the destination map
  OsmMapPtr tile = _tileMaps[maxEnvelopeIndex];
  CopyMapSubsetOp op(_map, element->getElementId());
  op.apply(tile);
  //  Update the list of elements already copied to maps
  const std::set<ElementId>& eids = op.getEidsCopied();
  _eidsCompleted.insert(eids.begin(), eids.end());

  if (_eidsCompleted.size() % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Sorted " << StringUtils::formatLargeNumber(_eidsCompleted.size()) << " elements.");
  }
}

void OsmMapSplitter::writeMaps(const QString& filename)
{
  vector<QString> outputFilenames;
  //  Use the filename for the first output
  outputFilenames.push_back(filename);
  QUrl url(filename);
  if (url.scheme() == MetadataTags::HootApiDbScheme())
  {
    //  For Hoot databases, just add the tile number to the map name
    for (size_t i = 1; i < _tileMaps.size(); ++i)
      outputFilenames.push_back(QString("%1-%2").arg(filename).arg(i, 5, 10, QChar('0')));
  }
  else if (url.scheme() == "")
  {
    QFileInfo info(filename);
    QString ext = info.completeSuffix();
    QString filepath = filename.left(filename.size() - (ext.size() + 1));
    //  For local files, add the tile number before the extension
    for (size_t i = 1; i < _tileMaps.size(); ++i)
      outputFilenames.push_back(QString("%1-%2.%3").arg(filepath).arg(i, 5, 10, QChar('0')).arg(ext));
  }
  else
  {
    LOG_ERROR("OsmMapSplitter - Unsupported file type: " << filename);
    return;
  }
  //  Now save off each map
  for (size_t index = 0; index < outputFilenames.size(); ++index)
    OsmMapWriterFactory::write(_tileMaps[index], outputFilenames[index]);
}

}

