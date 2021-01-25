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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBREADER_H
#define OSMAPIDBREADER_H

#include <hoot/core/io/ApiDbReader.h>
#include <hoot/core/io/OsmApiDb.h>

namespace hoot
{

/**
 * Reads from an OSM API database.
 */
class OsmApiDbReader : public ApiDbReader
{
public:

  static QString className() { return "hoot::OsmApiDbReader"; }

  OsmApiDbReader();
  virtual ~OsmApiDbReader();

  virtual void open(const QString& urlStr) override;

  virtual void setConfiguration(const Settings &conf) override;

  virtual QString supportedFormats() override { return MetadataTags::OsmApiDbScheme() + "://"; }

protected:

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) override;
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) override;
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map) override;

  virtual std::shared_ptr<ApiDb> _getDatabase() const override { return _database; }

private:

  //for white box testing
  friend class ServiceOsmApiDbBulkInserterTest;
  friend class ServicesDbTestUtils;
  friend class ServiceWriteOsmSqlStatementsDriverTest;

  std::shared_ptr<OsmApiDb> _database;

  /*
   * The element ID is passed in separately from the element here, as the element may have been
   * set with a remapped ID, and we need to use the original ID to retrieve the element's tags.
   */
  void _parseAndSetTagsOnElement(const ElementId& elementId, const ElementPtr& element);
};

}

#endif // OSMAPIDBREADER_H
