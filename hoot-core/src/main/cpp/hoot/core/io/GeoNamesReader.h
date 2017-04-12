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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef GEONAMESREADER_H
#define GEONAMESREADER_H

// hoot
#include <hoot/core/io/PartialOsmMapReader.h>

// Qt
#include <QFile>

namespace hoot
{

class GeoNamesReader : public PartialOsmMapReader
{
public:
  static string className() { return "hoot::GeoNamesReader"; }

  GeoNamesReader();

  virtual void close();

  virtual void initializePartial() {}

  virtual void finalizePartial() {}

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  virtual bool hasMoreElements();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  virtual ElementPtr readNextElement();

  void setDefaultAccuracy(Meters circularError) { _circularError = circularError; }

  void setDefaultStatus(Status s) { _status = s; }

  virtual void setUseDataSourceIds(bool useDataSourceIds) { _useDataSourceIds = useDataSourceIds; }

private:
  Meters _circularError;
  QStringList _columns;
  QList<int> _convertColumns;
  QFile _fp;
  Status _status;
  QHash<QString, QString> _strings;
  int _LATITUDE;
  int _LONGITUDE;
  int _GEONAMESID;
  bool _useDataSourceIds;
  mutable boost::shared_ptr<OGRSpatialReference> _wgs84;


  const QString& _saveMemory(const QString& s);
};

}

#endif // GEONAMESREADER_H
