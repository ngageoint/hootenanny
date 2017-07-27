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

class OsmApiDbSqlStatementFormatter;

/**
 * The driver that launches the Hadoop job for writing OSM API database SQL statements, both
 * to a file and to a database.  See big-convert documentation for more details.
 */
class WriteOsmSqlStatementsDriver : public Driver, public Configurable
{
public:

  WriteOsmSqlStatementsDriver();
  ~WriteOsmSqlStatementsDriver();

  virtual void setConfiguration(const Settings& conf);

  //The OsmMapWriter interface doesn't really fit here, due to the write method it has,
  //but we'll go ahead and mimic it with some of the same mehods.
  bool isSupported(QString urlStr);
  void open(QString url);
  void write(const QString inputMapFile);
  void close();

  void setFileOutputElementBufferSize(long size) { _fileOutputElementBufferSize = size; }
  void setOutputFilesCopyLocation(QString loc) { _outputFileCopyLocation = loc; }
  void setChangesetUserId(long id) { _changesetUserId = id; }
  void setExecSqlWithMapreduce(bool exec) { _execSqlWithMapreduce = exec; }
  void setNumReduceTasks(int numTasks) { _numReduceTasks = numTasks; }

private:

  OsmApiDb _database;
  long _fileOutputElementBufferSize;
  QString _outputFileCopyLocation;
  boost::shared_ptr<OsmApiDbSqlStatementFormatter> _sqlFormatter;
  QString _outputDelimiter;
  long _changesetUserId;
  QString _output;
  //Leaving this optional in case there end up being scalability problems writing to the db, but
  //if there aren't, then this setting can be removed;
  bool _execSqlWithMapreduce;
  //mostly for performance experimenting purposes; may not be needed forever
  int _numReduceTasks;

  /*
   * launches the hadoop job
   */
  void _runElementSqlStatementsWriteJob(const std::string& input, const std::string& output);
  /*
   * determines where the output sql file will go based on whether the output sql file is to be
   * retained or not
   */
  QString _getSqlFileOutputLocation(const QString hdfsOutput,
                                    const QString userSpecifiedOutput) const;
  /*
   * appends a single changeset ref'd by all the output elements to the output sql file
   */
  void _writeChangesetToSqlFile(const QString sqlFileLocation);
  bool _destinationIsDatabase(const QString output) const;
  /*
   * parses aux output files written during the sql statements job that have entries for element
   * counts encountered by type, sums them, and then appends db sequence id update statements to
   * the output sql file if the final output is a sql file or the sql file is to be retained OR
   * execs the sequence id updates against the database if the final output destination is a
   * database
   */
  void _writeSequenceUpdateStatements(const QString elementCountsDir,
                                      const QString sqlFileLocation);
};

}

#endif // WRITEOSMSQLSTATEMENTSDRIVER_H
