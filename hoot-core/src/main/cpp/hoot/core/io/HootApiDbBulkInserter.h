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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HOOTAPIDBBULKINSERTER_H
#define HOOTAPIDBBULKINSERTER_H

// hoot
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

  static QString className() { return "HootApiDbBulkInserter"; }

  HootApiDbBulkInserter();
  ~HootApiDbBulkInserter() override;

  bool isSupported(const QString& url) const override;
  void open(const QString& url) override;

  void finalizePartial() override;
  void writePartial(const ConstNodePtr& node) override;
  void writePartial(const ConstWayPtr& way) override;
  void writePartial(const ConstRelationPtr& relation) override;

  void setConfiguration(const Settings& conf) override;

  long getMapId() const { return _database.getMapId(); }

  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }
  void setUserEmail(const QString& email) { _userEmail = email; }
  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }
  void setCopyBulkInsertActivated(bool activated) { _copyBulkInsertActivated = activated; }

  QString supportedFormats() const override { return MetadataTags::HootApiDbScheme() + "://"; }

protected:

  unsigned int _numberOfFileDataPasses() const override;

  unsigned long _getTotalRecordsWritten() const override;
  unsigned long _getTotalFeaturesWritten() const override;

  //creates the output files containing the data
  void _createNodeOutputFiles() override;
  QStringList _createSectionNameList() override;
  void _createWayOutputFiles() override;
  void _createRelationOutputFiles() override;

  void _writeChangeset() override;
  void _writeRelation(const unsigned long relationDbId, const Tags& tags,
                              const unsigned long version);
  void _writeRelationMember(const unsigned long sourceRelationDbId,
                                    const RelationData::Entry& member,
                                    const unsigned long memberDbId,
                                    const unsigned int memberSequenceIndex,
                                    const unsigned long version) override;
  void _writeWay(const unsigned long wayDbId, const Tags& tags, const unsigned long version);
  void _writeWayNodes(const unsigned long wayId,
                              const std::vector<long>& wayNodeIds,
                              const unsigned long version) override;
  void _writeNode(const ConstNodePtr& node, const unsigned long nodeDbId) override;

  void _writeCombinedSqlFile() override;
  void _writeDataToDb() override;
  void _writeDataToDbPsql() override;

  bool _destinationIsDatabase() const  override{ return true; }

  void _incrementChangesInChangeset() override;

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
