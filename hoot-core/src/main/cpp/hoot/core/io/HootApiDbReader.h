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
#ifndef HOOTAPIDBREADER_H
#define HOOTAPIDBREADER_H

#include "PartialOsmMapReader.h"
#include "ApiDbReader.h"
#include "HootApiDb.h"

// hoot
#include <hoot/core/util/Configurable.h>

#include "EnvelopeProvider.h"

namespace hoot
{

class HootApiDbReader : public ApiDbReader, public PartialOsmMapReader, public Configurable,
    public EnvelopeProvider
{
public:

  static std::string className() { return "hoot::HootApiDbReader"; }

  HootApiDbReader();

  virtual ~HootApiDbReader();

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

  virtual void initializePartial();

  /**
   * The read command called after open.
   */
  virtual void read(boost::shared_ptr<OsmMap> map);

  virtual void finalizePartial();

  void close();

  /**
   * Called after open. This will read the bounds of the specified layer in a relatively efficient
   * manner. (e.g. SELECT min(x)...)
   */
  virtual Envelope calculateEnvelope() const;

  virtual void setConfiguration(const Settings &conf);

  void setUserEmail(const QString& email) { _email = email; }

  virtual bool hasMoreElements();

  virtual boost::shared_ptr<Element> readNextElement();

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

protected:

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map);
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map);
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map);

  virtual boost::shared_ptr<ApiDb> _getDatabase() const { return _database; }

private:

  boost::shared_ptr<HootApiDb> _database;
  boost::shared_ptr<QSqlQuery> _elementResultIterator;
  QString _email;
  ElementType _selectElementType;

  boost::shared_ptr<Element> _nextElement;

  const ElementType _getCurrentSelectElementType() const;

  void _read(boost::shared_ptr<OsmMap> map, const ElementType& elementType);

  boost::shared_ptr<Element> _getElementUsingIterator();

  /**
   * Converts a query result to an OSM element
   */
  boost::shared_ptr<Element> _resultToElement(QSqlQuery& resultIterator, const ElementType& elementType,
                                       OsmMap& map);
};

}

#endif // HOOTAPIDBREADER_H
