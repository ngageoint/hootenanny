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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef GEONAMESREADER_H
#define GEONAMESREADER_H

// hoot
#include <hoot/core/io/PartialOsmMapReader.h>

namespace hoot
{

class GeoNamesReader : public PartialOsmMapReader
{
public:

  static QString className() { return "GeoNamesReader"; }

  GeoNamesReader();
  ~GeoNamesReader() override = default;

  void close() override;
  void initializePartial() override { }
  void finalizePartial() override { }

  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  bool hasMoreElements() override;
  bool isSupported(const QString& url) const override;
  void open(const QString& url) override;
  ElementPtr readNextElement() override;

  void setDefaultAccuracy(Meters circularError) { _defaultCircularError = circularError; }
  void setDefaultStatus(const Status& s) override { _status = s; }
  void setUseDataSourceIds(bool useDataSourceIds) override { _useDataSourceIds = useDataSourceIds; }

  QString supportedFormats() const override { return ".geonames"; }

private:

  int _GEONAMESID;
  int _LATITUDE;
  int _LONGITUDE;
  int _maxSaveMemoryStrings;
  Meters _defaultCircularError;
  bool _useDataSourceIds;

  QStringList _columns;
  QFile _fp;
  Status _status;
  QHash<QString, QString> _strings;
  mutable std::shared_ptr<OGRSpatialReference> _wgs84;

  QString _saveMemory(const QString& s);
};

}

#endif // GEONAMESREADER_H
