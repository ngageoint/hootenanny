/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef WRITEOSMSQLSTATEMENTSDRIVER_H
#define WRITEOSMSQLSTATEMENTSDRIVER_H

// hoot
#include <hoot/hadoop/Driver.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/util/Configurable.h>

// Standard
#include <string>

namespace hoot
{

using namespace std;

class OsmApiDbSqlStatementFormatter;

class WriteOsmSqlStatementsDriver : public Driver, public Configurable
{
public:

  WriteOsmSqlStatementsDriver();
  ~WriteOsmSqlStatementsDriver();

  void write(const QString inputMapFile);
  void close();

  virtual void setConfiguration(const Settings& conf);

  //The OsmMapWriter interface doesn't really fit here, due to the write method it has,
  //but we'll go ahead and partially mimic it with open and isSupported.
  bool isSupported(QString urlStr);
  void open(QString url);

  void setFileOutputElementBufferSize(long size) { _fileOutputElementBufferSize = size; }
  void setOutputFilesCopyLocation(QString loc) { _outputFileCopyLocation = loc; }
  void setChangesetUserId(long id) { _changesetUserId = id; }
  void setExecSqlWithMapreduce(bool exec) { _execSqlWithMapreduce = exec; }
  void setNumReduceTasks(int numTasks) { _numReduceTasks = numTasks; }

private:

  OsmApiDb _database;
  long _fileOutputElementBufferSize;
  QString _outputFileCopyLocation;
  shared_ptr<OsmApiDbSqlStatementFormatter> _sqlFormatter;
  QString _outputDelimiter;
  long _changesetUserId;
  QString _output;
  bool _execSqlWithMapreduce;
  int _numReduceTasks;

  void _runElementSqlStatementsWriteJob(const string& input, const string& output);
  void _runChangesetSqlStatementsWriteJob(const string& input, const string& output);
  QString _getSqlFileOutputLocation(const QString hdfsOutput,
                                    const QString userSpecifiedOutput) const;
  void _writeChangesetToSqlFile(const QString sqlFileLocation);
  bool _destinationIsDatabase(const QString output) const;
};

}

#endif // WRITEOSMSQLSTATEMENTSDRIVER_H
