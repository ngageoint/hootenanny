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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmGeoJsonWriter.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGeoJsonWriter)

OsmGeoJsonWriter::OsmGeoJsonWriter(int precision)
  : OsmGeoJsonWriterBase(precision)
{
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

void OsmGeoJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;
  if (_out->isWritable() == false)
    throw HootException("Please open the file before attempting to write.");

  _write("{");
  _writeKvp("generator", "Hootenanny");
  _write(",");
  _writeKvp("type", "FeatureCollection");
  _write(",");
  _write("\"bbox\": ");
  _write(_getBbox());
  _write(",");
  if (_useTaskingManagerFormat && map->getSource() != "")
  {
    _writeKvp(MetadataTags::Source(), map->getSource());
    _write(",");
  }
  _write("\"features\": [", true);
  _firstElement = true;
  _writeNodes();
  _writeWays();
  _writeRelations();
  _writeLn("]");
  _writeLn("}");
  close();
}

QString OsmGeoJsonWriter::toString(const ConstOsmMapPtr& map)
{
  QBuffer buf;

  if (!buf.open(QBuffer::WriteOnly))
    throw InternalErrorException(QObject::tr("Error opening QBuffer for writing. Odd."));

  _out = &buf;
  write(map);
  _out = nullptr;

  return QString::fromUtf8(buf.buffer());
}

void OsmGeoJsonWriter::setConfiguration(const Settings& conf)
{
  OsmGeoJsonWriterBase::setConfiguration(conf);
  _useTaskingManagerFormat = ConfigOptions(conf).getJsonOutputTaskingManagerAoi();
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

QString OsmGeoJsonWriter::_getBbox() const
{ 
  Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_map);
  return QString("[%1, %2, %3, %4]")
      .arg(QString::number(bounds.getMinX(), 'g', 5), QString::number(bounds.getMinY(), 'g', 5),
           QString::number(bounds.getMaxX(), 'g', 5), QString::number(bounds.getMaxY(), 'g', 5));
}

void OsmGeoJsonWriter::_writeMeta(ConstElementPtr e)
{
  _write("\"meta\": {");
  _writeKvp("timestamp", DateTimeUtils::toTimeString(e->getTimestamp())); _write(",");
  _writeKvp("version", e->getVersion()); _write(",");
  _writeKvp("visible", e->getVisible() ? "true" : "false");
  _write("}");
}

void OsmGeoJsonWriter::_writeNodes()
{
  NoInformationCriterion crit;
  vector<long> nids;
  const NodeMap& nodes = _map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (!crit.isSatisfied(_map->getNode(it->first)))
      nids.push_back(it->first);
  }
  // sort the values to give consistent results.
  sort(nids.begin(), nids.end(), greater<long>());
  for (auto node_id : nids)
  {
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    _writeNode(_map->getNode(node_id));

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}


void OsmGeoJsonWriter::_writeWays()
{
  const WayMap& ways = _map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr w = it->second;
    //  Skip any ways that have parents
    set<ElementId> parents = _map->getParents(w->getElementId());
    if (!parents.empty())
      continue;
    if (w.get() == nullptr)
      continue;
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    //  Make sure that building ways are "complete"
    const vector<long>& nodes = w->getNodeIds();
    bool valid = true;
    if (AreaCriterion().isSatisfied(w))
    {
      for (auto node_id : nodes)
      {
        ConstNodePtr node = _map->getNode(node_id);
        if (node.get() == nullptr)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in geojson if valid
    if (valid)
      _writeWay(w);
    else
    {
      for (auto node_id : nodes)
      {
        ConstNodePtr node = _map->getNode(node_id);
        if (node.get() != nullptr)
        {
          if (_firstElement)
            _firstElement = false;
          else
            _write(",", true);
          _writeNode(node);
        }
      }
    }

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeRelations()
{
  const RelationMap& relations = _map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    ConstRelationPtr r = it->second;
    //  Write out the relation (and all children) in geojson
    _write("{");
    _writeFeature(r);
    _write(",");
    _write("\"geometry\": {");
    _writeGeometry(r);
    _write("}");
    _write("}", false);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

}
