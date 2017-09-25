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
#ifndef HOOTAPIDBBULKINSERTER_H
#define HOOTAPIDBBULKINSERTER_H

#include <hoot/core/io/OsmApiDbBulkInserter.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

class HootApiDbSqlStatementFormatter;

/**
 * A Hooteanny API database writer that should perform faster than HootApiDbWriter when creating new
 * elements in a new database only.  See OsmApiDbBulkInserter.  This class implements the offline
 * workflow only and does not allow for writing to a SQL file.
 *
 * TODO: Having this class inheriting from OsmApiDbBulkInserter is not the cleanest approach but
 * allowed for reducing a lot of redundant code up front without involving a massive amount of
 * refactoring.  The better long term approach is probably to create a ApiDbBulkinserter base class
 * and have both this class and OsmApiDbBulkInserter inherit from it.
 */
class HootApiDbBulkInserter : public OsmApiDbBulkInserter
{

public:

  static std::string className() { return "hoot::HootApiDbBulkInserter"; }

  static unsigned int logWarnCount;

  HootApiDbBulkInserter();
  virtual ~HootApiDbBulkInserter();

  virtual bool isSupported(QString url);
  virtual void open(QString url);

  virtual void finalizePartial();
  virtual void writePartial(const ConstNodePtr& node);
  virtual void writePartial(const ConstWayPtr& way);
  virtual void writePartial(const ConstRelationPtr& relation);

  virtual void setConfiguration(const Settings& conf);

  long getMapId() const { return _database.getMapId(); }

  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }
  void setUserEmail(QString email) { _userEmail = email; }
  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }
  void setFastBulkInsertActivated(bool activated) { _fastBulkInsertActivated = activated; }

protected:

 virtual unsigned int _numberOfFileDataPasses() const;

 virtual unsigned long _getTotalRecordsWritten() const;
 virtual unsigned long _getTotalFeaturesWritten() const;

 //creates the output files containing the data
 virtual void _createNodeOutputFiles();
 virtual QStringList _createSectionNameList();
 virtual void _createWayOutputFiles();
 virtual void _createRelationOutputFiles();

  virtual void _writeChangeset();
  virtual void _writeRelation(const unsigned long relationDbId, const Tags& tags);
  virtual void _writeRelationMember(const unsigned long sourceRelationDbId,
                            const RelationData::Entry& member,
                            const unsigned long memberDbId,
                            const unsigned int memberSequenceIndex);
  virtual void _writeWay(const unsigned long wayDbId, const Tags& tags);
  virtual void _writeWayNodes(const unsigned long wayId, const std::vector<long>& wayNodeIds);
  virtual void _writeNode(const ConstNodePtr& node, const unsigned long nodeDbId);

  virtual void _writeCombinedSqlFile();
  virtual void _writeDataToDb();
  virtual void _writeDataToDbPsql();

  virtual bool _destinationIsDatabase() const { return true; }

  virtual void _incrementChangesInChangeset();

private:

  QString _userEmail;
  bool _createUserIfNotFound;
  bool _overwriteMap;

  HootApiDb _database;
  boost::shared_ptr<HootApiDbSqlStatementFormatter> _sqlFormatter;

  bool _fastBulkInsertActivated;

  Tags _changesetTags;

  void _getOrCreateMap();
};

}

#endif // HOOTAPIDBBULKINSERTER_H
