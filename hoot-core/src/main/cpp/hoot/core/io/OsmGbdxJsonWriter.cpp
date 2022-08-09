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
#include "OsmGbdxJsonWriter.h"

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
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
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

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGbdxJsonWriter)

OsmGbdxJsonWriter::OsmGbdxJsonWriter(int precision)
  : OsmGeoJsonWriterBase(precision),
    _fileNumber(0)
{
  _writeHootFormat = false;
}

void OsmGbdxJsonWriter::open(const QString& path)
{
  QFileInfo fi(path);
  _outputDir = fi.absoluteDir();
  _outputFileName = fi.baseName();

  if (!_outputDir.exists() && !FileUtils::makeDir(_outputDir.path()))
    throw HootException("Error creating directory for writing.");
}

void OsmGbdxJsonWriter::_newOutputFile()
{
  // Close the old file and open a new one
  if (_fp.isOpen())
    close();

  // The output has had a few changes.....
  QString url = _outputDir.filePath(QString("%1_00_%2.json").arg(_outputFileName).arg(_fileNumber++));

  // If the file exists, increment the middle _00_ in the name.
  // NOTE: This assumes that there can be a maximum of 10 copies of a filename....
  if (QFile::exists(url))
  {
    int inc = 0;
    while (QFile::exists(url))
    {
      inc++;
      url = _outputDir.filePath(QString("%1_%2_%3.json").arg(_outputFileName).arg(inc,2,10,QChar('0')).arg(_fileNumber));
    }
  }

  _fp.setFileName(url);

  if (!_fp.open(QIODevice::WriteOnly | QIODevice::Text))
    throw HootException(QObject::tr("Error opening %1 for writing").arg(url));

  _out = &_fp;
}

void OsmGbdxJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;

  _writeNodes();
  _writeWays();
  _writeRelations();

  // This will move out of here eventually.
  close();
}

void OsmGbdxJsonWriter::_writeNodes()
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
    _newOutputFile();
    _writeNode(_map->getNode(node_id));
  }
}

void OsmGbdxJsonWriter::_writeWays()
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
    //  Write out the way in Gbdxjson if valid
    if (valid)
    {
      _newOutputFile();
      _writeWay(w);
    }
    else
    {
      for (auto node_id : nodes)
      {
        ConstNodePtr node = _map->getNode(node_id);
        if (node.get() != nullptr)
        {
          _newOutputFile();
          _writeNode(node);
        }
      }
    }
  }
}

void OsmGbdxJsonWriter::_writeRelations()
{
  const RelationMap& relations = _map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr r = it->second;
    //  Write out the relation (and all children) in geojson
    _newOutputFile();
    _write("{");
    _writeFeature(r);
    _write(",");
    _write("\"geometry\": {");
    _writeGeometry(r);
    _write("}");
    _write("}", false);
  }
}

}
