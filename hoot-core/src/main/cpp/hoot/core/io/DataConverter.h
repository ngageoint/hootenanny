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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DATACONVERTER_H
#define DATACONVERTER_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Converts data from one Hootenanny supported format to another
 *
 * There may be opportunities to consolidate some of the code in the various conversion functions.
 */
class DataConverter
{

public:

  static unsigned int logWarnCount;

  DataConverter();

  void convert(const QStringList inputs, const QString output);

  void setTranslation(const QString translation) { _translation = translation; }
  void setColumns(const QStringList columns) { _columns = columns; }
  void setColsArgSpecified(const bool specified) { _colsArgSpecified = specified; }
  void setFeatureReadLimit(const int limit) { _featureReadLimit = limit; }

private:

  QString _translation;
  QStringList _columns;
  bool _colsArgSpecified;
  int _featureReadLimit;

  void _convertOsmToShp(const QString input, const QString output);
  void _convertOsmToOgr(const QString input, const QString output);
  void _convertOgrToOsm(const QStringList inputs, const QString output);
  void _convertOgrToOsm(const QString input, const QString output);
  void _generalConvert(const QString input, const QString output);

  /*
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   */
  bool _areValidStreamingOps(const QStringList ops);

  void _validateInput(const QStringList inputs, const QString output);

  void _convertSingle(const QStringList inputs, const QString output);
};

}

#endif // DATACONVERTER_H
