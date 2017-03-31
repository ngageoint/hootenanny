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
#ifndef OSMAPIDBREADER_H
#define OSMAPIDBREADER_H

#include "OsmMapReader.h"
#include "ApiDbReader.h"
#include "OsmApiDb.h"

// hoot
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class OsmApiDbReader : public ApiDbReader, public OsmMapReader, public Configurable
{
public:

  static std::string className() { return "hoot::OsmApiDbReader"; }

  OsmApiDbReader();

  virtual ~OsmApiDbReader();

  /**
   * Determines the reader's default element status. By default this is Invalid which specifies that
   * the file's status will be used.
   */
  virtual void setDefaultStatus(Status status) { _status = status; }

  /**
   * Determines whether the reader should use the element id's from the file being read
   */
  virtual void setUseDataSourceIds(bool useDataSourceIds) { _useDataSourceIds = useDataSourceIds; }

  virtual bool isSupported(QString urlStr);

  virtual void open(QString urlStr);

  /**
   * The read command called after open.
   */
  virtual void read(shared_ptr<OsmMap> map);

  void close();

  virtual void setConfiguration(const Settings &conf);

  void setUserEmail(const QString email) { _email = email; }

protected:

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map);
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map);
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map);

  virtual shared_ptr<ApiDb> _getDatabase() const { return _database; }

private:

  //for white box testing
  friend class ServiceOsmApiDbBulkWriterTest;
  friend class ServicesDbTestUtils;
  friend class WriteOsmSqlStatementsDriverTest;

  shared_ptr<OsmApiDb> _database;
  shared_ptr<QSqlQuery> _elementResultIterator;
  QString _email;

  long _osmElemId;
  ElementType _osmElemType;
  ElementType _selectElementType;

  void _read(OsmMapPtr map, const ElementType& elementType);

  void _addNodesForWay(vector<long> nodeIds, OsmMap& map);

  void _parseAndSetTagsOnElement(ElementPtr element);
};

}

#endif // OSMAPIDBREADER_H
