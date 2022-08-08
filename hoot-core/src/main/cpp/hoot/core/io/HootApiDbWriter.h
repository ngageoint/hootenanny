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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef HOOTAPIDBWRITER_H
#define HOOTAPIDBWRITER_H

// hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>

// Tgs
#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class HootApiDbWriter : public PartialOsmMapWriter, public Configurable
{
public:

  static QString className() { return "HootApiDbWriter"; }

  HootApiDbWriter();
  ~HootApiDbWriter();

  void close() override;
  bool isSupported(const QString& urlStr) const override;
  void open(const QString& urlStr) override;
  QString supportedFormats() const override { return MetadataTags::HootApiDbScheme() + "://"; }

  void finalizePartial() override;
  void writePartial(const ConstNodePtr& n) override;
  void writePartial(const ConstWayPtr& w) override;
  void writePartial(const ConstRelationPtr& r) override;

  void setConfiguration(const Settings &conf) override;

  void deleteMap(const QString& urlStr);

  long getMapId() const { return _hootdb.getMapId(); }

  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }
  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }
  void setTextStatus(bool textStatus) { _textStatus = textStatus; }
  void setIncludeCircularError(bool includeCircularError)
  { _includeCircularError = includeCircularError; }
  /**
   * If set to true (the default) then all IDs are remapped into new IDs. This is appropriate if
   * any of the input IDs are non-positive.
   */
  void setRemap(bool remap) { _remapIds = remap; }
  void setUserEmail(const QString& email) { _userEmail = email; }
  void setJobId(const QString& id) { _jobId = id; }
  void setPreserveVersionOnInsert(bool preserve) { _preserveVersionOnInsert = preserve; }
  void setCopyBulkInsertActivated(bool activated) { _copyBulkInsertActivated = activated; }

protected:

  using IdRemap = Tgs::BigMap<long, long>;
  IdRemap _nodeRemap;
  IdRemap _relationRemap;
  IdRemap _wayRemap;

  std::set<long> _sourceNodeIds;
  std::set<long> _sourceWayIds;
  std::set<long> _sourceRelationIds;

  QString _outputMappingFile;

  HootApiDb _hootdb;

  unsigned long _nodesWritten;
  unsigned long _waysWritten;
  unsigned long _relationsWritten;

  bool _remapIds;

  /**
   * Return the remapped ID for the specified element if it exists
   * @param eid The ID for the ID from the source data
   * @return unique ID of the element in the database namespace
   *
   * @note If there is no mapping for the requested element ID in the
   *  database, a new one is created which is guaranteed to be unique
   */
  virtual long _getRemappedElementId(const ElementId& eid);

  virtual std::vector<long> _remapNodes(const std::vector<long>& nids);

  void _addElementTags(const std::shared_ptr<const Element>& e, Tags& t) const;

  /**
   * Counts the change and if necessary closes the old changeset and starts a new one.
   */
  void _countChange();

private:

  bool _createUserIfNotFound;
  bool _overwriteMap;
  QString _userEmail;
  bool _includeIds;
  bool _textStatus;
  bool _includeCircularError;
  QString _jobId;
  // If true the version of the element will be retained when writing to the db. Otherwise, the
  // version is reset to the initial version of 1.
  bool _preserveVersionOnInsert;

  bool _open;

  bool _copyBulkInsertActivated;

  /**
   * Close the current changeset and start a new one.
   */
  void _startNewChangeSet();

  long _openDb(const QString& urlStr);
  QString _getMapNameFromUrl(const QString& urlStr) const;
};

}

#endif // HOOTAPIDBWRITER_H
