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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOTAPIDBWRITER_H
#define HOOTAPIDBWRITER_H

#include "PartialOsmMapWriter.h"
#include "HootApiDb.h"

// hoot
#include <hoot/core/util/Configurable.h>

// Tgs
#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class HootApiDbWriter : public PartialOsmMapWriter, public Configurable
{
public:

  static std::string className() { return "hoot::HootApiDbWriter"; }
  static QString emailKey() { return "hootapi.db.writer.email"; }
  static QString overwriteMapKey() { return "hootapi.db.writer.overwrite.map"; }

  HootApiDbWriter();

  virtual ~HootApiDbWriter();

  void close();

  virtual void finalizePartial();

  long getMapId() const { return _hootdb.getMapId(); }

  virtual bool isSupported(QString urlStr);

  virtual void open(QString urlStr);

  virtual void deleteMap(QString urlStr);

  virtual void setConfiguration(const Settings &conf);

  void setCreateUser(bool createIfNotFound) { _createUserIfNotFound = createIfNotFound; }

  void setOverwriteMap(bool overwriteMap) { _overwriteMap = overwriteMap; }

  /**
   * If set to true (the default) then all IDs are remapped into new IDs. This is appropriate if
   * any of the input IDs are non-positive.
   */
  void setRemap(bool remap) { _remapIds = remap; }

  void setUserEmail(QString email) { _userEmail = email; }

  virtual void writePartial(const shared_ptr<const Node>& n);

  virtual void writePartial(const shared_ptr<const Way>& w);

  virtual void writePartial(const shared_ptr<const Relation>& r);

protected:

  /**
   * Return the remapped ID for the specified element if it exists
   * @param eid The ID for the ID from the source data
   * @return unique ID of the element in the database namespace
   *
   * @note If there is no mapping for the requested element ID in the
   *  database, a new one is created which is guaranteed to be unique
   */
  virtual long _getRemappedElementId(const ElementId& eid);

  virtual vector<long> _remapNodes(const vector<long>& nids);

  void _addElementTags(const shared_ptr<const Element>& e, Tags& t);

  /**
   * Counts the change and if necessary closes the old changeset and starts a new one.
   */
  void _countChange();

  typedef Tgs::BigMap<long, long> IdRemap;
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

private:

  bool _createUserIfNotFound;
  bool _overwriteMap;
  QString _userEmail;

  bool _open;

  set<long> _openDb(QString& urlStr);

  void _overwriteMaps(const QString& mapName, const set<long>& mapIds);

  /**
   * Close the current changeset and start a new one.
   */
  void _startNewChangeSet();
};

}

#endif // HOOTAPIDBWRITER_H
