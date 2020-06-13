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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOTAPIDBBULKINSERTER_H
#define HOOTAPIDBBULKINSERTER_H

#include <hoot/core/io/OsmApiDbBulkInserter.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

class HootApiDbSqlStatementFormatter;

/**
 * A Hooteanny API database writer that performs faster than HootApiDbWriter but for use when
 * creating new elements in a new database only.  See OsmApiDbBulkInserter.  This class implements
 * the offline workflow only and does not allow for writing to a SQL file.
 *
 * Note: The constraint and index disable settings have no effect on this class since Hoot API
 * database writing creates a brand new database for every layer that has no constraints to start
 * with.
 *
 * Having this class inheriting from OsmApiDbBulkInserter is not the cleanest approach but
 * allowed for reducing a lot of redundant code up front without involving a massive amount of
 * refactoring work.  The better long term approach is probably to create a ApiDbBulkinserter base
 * class and have both this class and OsmApiDbBulkInserter inherit from it.
 */
class HootApiDbBulkInserter : public OsmApiDbBulkInserter
{

public:

  static std::string className() { return "hoot::HootApiDbBulkInserter"; }

  HootApiDbBulkInserter();
  virtual ~HootApiDbBulkInserter();

  virtual bool isSupported(const QString& url) override;
  virtual void open(const QString& url) override;

  virtual void finalizePartial();
  virtual void writePartial(const ConstNodePtr& node) override;
  virtual void writePartial(const ConstWayPtr& way) override;
  virtual void writePartial(const ConstRelationPtr& relation) override;

  virtual void setConfiguration(const Settings& conf) override;

  long getMapId() const { return _database.getMapId(); }

  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }
  void setUserEmail(const QString& email) { _userEmail = email; }
  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }
  void setCopyBulkInsertActivated(bool activated) { _copyBulkInsertActivated = activated; }

  virtual QString supportedFormats() override { return MetadataTags::HootApiDbScheme() + "://"; }

protected:

  virtual unsigned int _numberOfFileDataPasses() const;

  virtual unsigned long _getTotalRecordsWritten() const;
  virtual unsigned long _getTotalFeaturesWritten() const;

  //creates the output files containing the data
  virtual void _createNodeOutputFiles();
  virtual QStringList _createSectionNameList() override;
  virtual void _createWayOutputFiles();
  virtual void _createRelationOutputFiles();

  virtual void _writeChangeset();
  virtual void _writeRelation(const unsigned long relationDbId, const Tags& tags);
  virtual void _writeRelationMember(const unsigned long sourceRelationDbId,
                            const RelationData::Entry& member,
                            const unsigned long memberDbId,
                            const unsigned int memberSequenceIndex) override;
  virtual void _writeWay(const unsigned long wayDbId, const Tags& tags);
  virtual void _writeWayNodes(const unsigned long wayId, const std::vector<long>& wayNodeIds) override;
  virtual void _writeNode(const ConstNodePtr& node, const unsigned long nodeDbId) override;

  virtual void _writeCombinedSqlFile();
  virtual void _writeDataToDb();
  virtual void _writeDataToDbPsql();

  virtual bool _destinationIsDatabase() const { return true; }

  virtual void _incrementChangesInChangeset();

private:

  static int logWarnCount;

  QString _userEmail;
  bool _createUserIfNotFound;
  bool _overwriteMap;

  HootApiDb _database;
  std::shared_ptr<HootApiDbSqlStatementFormatter> _sqlFormatter;

  bool _copyBulkInsertActivated;

  Tags _changesetTags;

  void _getOrCreateMap();
};

}

#endif // HOOTAPIDBBULKINSERTER_H
