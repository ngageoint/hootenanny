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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmUtils.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/io/GeoNamesReader.h>
#include <hoot/core/util/Progress.h>

//Qt
#include <QDateTime>

namespace hoot
{

void OsmUtils::printNodes(const QString nodeCollectionName,
                          const QList<shared_ptr<const Node> >& nodes)
{
  if (Log::getInstance().getLevel() == Log::Debug)
  {
    LOG_DEBUG(nodeCollectionName);
    LOG_VARD(nodes.size());
    for (QList<shared_ptr<const Node> >::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
      shared_ptr<const Node> node = *it;
      LOG_VARD(node->toString());
    }
  }
}

const QList<long> OsmUtils::nodesToNodeIds(const QList<shared_ptr<const Node> >& nodes)
{
  QList<long> nodeIds;
  for (QList<shared_ptr<const Node> >::const_iterator it = nodes.constBegin();
       it != nodes.constEnd(); ++it)
  {
    shared_ptr<const Node> node = *it;
    nodeIds.append(node->getElementId().getId());
  }
  return nodeIds;
}

QList<shared_ptr<const Node> > OsmUtils::nodeIdsToNodes(const QList<long>& nodeIds,
                                                        shared_ptr<const OsmMap> map)
{
  QList<shared_ptr<const Node> > nodes;
  for (QList<long>::const_iterator it = nodeIds.constBegin(); it != nodeIds.constEnd(); ++it)
  {
    nodes.append(dynamic_pointer_cast<const Node>(map->getElement(ElementType::Node, *it)));
  }
  return nodes;
}

Coordinate OsmUtils::nodeToCoord(shared_ptr<const Node> node)
{
  return Coordinate(node->getX(), node->getY());
}

void OsmUtils::loadMap(shared_ptr<OsmMap> map, QString path, bool useFileId, Status defaultStatus)
{
  QStringList pathLayer = path.split(";");
  QString justPath = pathLayer[0];
  if (OgrReader::isReasonablePath(justPath))
  {
    OgrReader reader;
    Progress progress("OsmUtils");
    reader.setDefaultStatus(defaultStatus);
    reader.read(justPath, pathLayer.size() > 1 ? pathLayer[1] : "", map, progress);
  }
  else
  {
    OsmMapReaderFactory::read(map, path, useFileId, defaultStatus);
  }
}

void OsmUtils::saveMap(shared_ptr<const OsmMap> map, QString path)
{
  OsmMapWriterFactory::write(map, path);
}

QString OsmUtils::toTimeString(unsigned int timestamp)
{
  // convert time in seconds since epoch into timestamp string
  time_t tt = (time_t) timestamp;
  char buf[128];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&tt));
  return QString::fromUtf8(buf);
}

unsigned int OsmUtils::fromTimeString(QString timestamp)
{
  struct tm t;
  strptime(timestamp.toStdString().c_str(), "%Y-%m-%dT%H:%M:%SZ", &t);

  // calc time in seconds since epoch
  return (unsigned int)(t.tm_sec + t.tm_min*60 + t.tm_hour*3600 + t.tm_yday*86400 +
    (t.tm_year-70)*31536000 + ((t.tm_year-69)/4)*86400 -
    ((t.tm_year-1)/100)*86400 + ((t.tm_year+299)/400)*86400);
}

QString OsmUtils::currentTimeAsString()
{
  return QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ssZ");
}

}
