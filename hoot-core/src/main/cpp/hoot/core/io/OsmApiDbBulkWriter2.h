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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBBULKWRITER2_H
#define OSMAPIDBBULKWRITER2_H

#include <string>
#include <map>
#include <vector>

#include <QString>
#include <QTemporaryFile>
#include <QTextStream>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

using namespace boost;
using namespace std;

/**
 *
 */
class OsmApiDbBulkWriter2 : public PartialOsmMapWriter, public Configurable
{

public:

  static string className() { return "hoot::OsmApiDbBulkWriter2"; }

  static unsigned int logWarnCount;

  OsmApiDbBulkWriter2();

  virtual ~OsmApiDbBulkWriter2();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  void close();

  virtual void finalizePartial();

  virtual void writePartial(const ConstNodePtr& n);

  virtual void writePartial(const ConstWayPtr& w);

  virtual void writePartial(const ConstRelationPtr& r);

  virtual void setConfiguration(const Settings& conf);

  void setFileOutputLineBufferSize(long size) { _fileOutputLineBufferSize = size; }
  void setStatusUpdateInterval(long interval) { _statusUpdateInterval = interval; }
  //void setChangesetUserId(long id) { _changesetData.changesetUserId = id; }
  void setExecuteSql(bool exec) { _executeSql = exec; }
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }

private:

  // for white box testing.
  //friend class ServiceOsmApiDbBulkWriterTest;

  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  bool _executeSql;
  long _maxChangesetSize;

  void _logStats(const bool debug);
  void _executeElementSql();
  long _getTotalRecordsWritten() const;
};

}

#endif // OSMAPIDBBULKWRITER2_H
