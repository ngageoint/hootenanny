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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SparkChangesetReader.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

SparkChangesetReader::SparkChangesetReader()
{
  _jsonReader.setUseDataSourceIds(true);
  _xmlReader.setUseDataSourceIds(true);
}

SparkChangesetReader::~SparkChangesetReader()
{
  close();
}

void SparkChangesetReader::open(QString fileName)
{
  close();

  _file.setFileName(fileName);
  if (!_file.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(fileName));
  }
  LOG_DEBUG("Opened: " << fileName << ".");
}

boost::shared_ptr<OGRSpatialReference> SparkChangesetReader::getProjection() const
{
  if (!_wgs84)
  {
    _wgs84 = MapProjector::getInstance().createWgs84Projection();
  }
  return _wgs84;
}

void SparkChangesetReader::close()
{
  _file.close();
}

bool SparkChangesetReader::hasMoreChanges()
{
  if (!_file.isOpen())
  {
    throw HootException("No file has been opened.");
  }
  return !_file.atEnd();
}

Change SparkChangesetReader::readNextChange()
{
  QStringList lineParts = QString::fromUtf8(_file.readLine().constData()).split("\t");
  LOG_VART(lineParts);

  int nodePayloadIndex = 6;
  Change::ChangeType changeType;
  const QString parsedChangeType = lineParts[0];
  if (parsedChangeType == QLatin1String("A"))
  {
    changeType = Change::Create;
  }
  else if (parsedChangeType == QLatin1String("M"))
  {
    changeType = Change::Modify;
    nodePayloadIndex = 7;
  }
  else if (parsedChangeType == QLatin1String("D"))
  {
    changeType = Change::Delete;
  }
  else
  {
    throw HootException("Unsupported change type: " + parsedChangeType);
  }
  LOG_VART(changeType);
  LOG_VART(nodePayloadIndex);

  LOG_VART(lineParts[nodePayloadIndex]);
  OsmMapPtr tmpMap(new OsmMap());
  if (lineParts[nodePayloadIndex].startsWith("{"))
  {
    //json - don't use this until #1772 is fixed
    tmpMap = _jsonReader.loadFromString(lineParts[nodePayloadIndex]);
  }
  else
  {
    _xmlReader.readFromString(lineParts[nodePayloadIndex], tmpMap);
  }
  if (tmpMap->getWayCount() > 0 || tmpMap->getRelationCount() > 0)
  {
    throw HootException("Only nodes are supported.");
  }
  assert(tmpMap->getNodeCount() == 1);
  NodePtr node = tmpMap->getNodes().begin()->second;
  LOG_VART(node);

  return Change(changeType, node);
}


}
