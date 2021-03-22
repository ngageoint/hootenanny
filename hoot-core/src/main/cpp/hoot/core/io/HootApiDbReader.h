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
#ifndef HOOTAPIDBREADER_H
#define HOOTAPIDBREADER_H

#include <hoot/core/io/ApiDbReader.h>
#include <hoot/core/io/EnvelopeProvider.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

/**
 * Reads from a Hootenanny customized version of the OSM API database.
 */
class HootApiDbReader : public ApiDbReader
{
public:

  static QString className() { return "hoot::HootApiDbReader"; }

  HootApiDbReader();

  virtual ~HootApiDbReader();

  /**
   * Called after open. This will read the bounds of the specified layer in a relatively efficient
   * manner. (e.g. SELECT min(x)...)
   */
  /**
   * TODO: Implement EnvelopeProvider
   */
  virtual geos::geom::Envelope calculateEnvelope() const;

  void setConfiguration(const Settings& conf) override;

  void open(const QString& urlStr) override;

  void read(const OsmMapPtr& map) override;

protected:

  NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) override;
  WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) override;
  RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map) override;

  std::shared_ptr<ApiDb> _getDatabase() const override { return _database; }

  QString supportedFormats() { return MetadataTags::HootApiDbScheme() + "://"; }

private:

  //for white box testing
  friend class ServiceHootApiDbBulkInserterTest;

  std::shared_ptr<HootApiDb> _database;
};

}

#endif // HOOTAPIDBREADER_H
