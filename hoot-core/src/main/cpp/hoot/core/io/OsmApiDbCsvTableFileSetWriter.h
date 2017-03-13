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
#ifndef OSMAPIDBCSVTABLEFILESETWRITER_H
#define OSMAPIDBCSVTABLEFILESETWRITER_H

#include <string>

#include <QString>
#include <QMap>
#include <QTextStream>
#include <QFile>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

using namespace boost;
using namespace std;

/**
 * Writes to individual CSV files, one for each table type in an OSM API database.
 */
class OsmApiDbCsvTableFileSetWriter : public PartialOsmMapWriter, public Configurable
{

public:

  static string className() { return "hoot::OsmApiDbCsvTableFileSetWriter"; }

  static unsigned int logWarnCount;

  OsmApiDbCsvTableFileSetWriter();

  virtual ~OsmApiDbCsvTableFileSetWriter();

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
  void setMaxChangesetSize(long size) { _maxChangesetSize = size; }

  QStringList getOutputFileNames() const;

  void logStats(const bool debug);

  long getTotalRecordsWritten() const;

private:

  // for white box testing.
  //friend class ServiceOsmApiDbBulkWriterTest;

  QMap<QString, shared_ptr<QFile> > _outputFiles;
  QMap<QString, shared_ptr<QTextStream> > _outputStreams;
  QMap<QString, long> _numRecordsWrittenByType;
  long _unflushedRecords;
  long _fileOutputLineBufferSize;
  long _statusUpdateInterval;
  long _maxChangesetSize;

  void _init(const QString outputUrl);
  void _writeTags(ConstElementPtr element);
  QString _formatPotentiallyLargeNumber(const long number);

};

}

#endif // OSMAPIDBCSVTABLEFILESETWRITER_H
