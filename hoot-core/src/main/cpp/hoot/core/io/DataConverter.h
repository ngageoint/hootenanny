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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DATACONVERTER_H
#define DATACONVERTER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QStringList>
#include <QQueue>

namespace hoot
{

/**
 * Converts data from one Hootenanny supported format to another
 *
 * OGR formats are handled with custom logic and all other formats are handled generically, with
 * the exception of shape files written with explicitly specified columns.
 */
class DataConverter : public Configurable
{

public:

  static int logWarnCount;
  static const QString JOB_SOURCE;

  DataConverter();
  ~DataConverter() = default;

  void setConfiguration(const Settings& conf) override;

  /**
   * Converts multiple datasets from format to a single output format
   *
   * @param inputs the inputs to convert
   * @param output the converted output path
   */
  void convert(const QStringList& inputs, const QString& output);

  /**
   * Converts a dataset from format to another output format
   *
   * @param input the input to convert
   * @param output the converted output path
   */
  void convert(const QString& input, const QString& output);

  void setTranslation(const QString& translation);
  void setShapeFileColumns(const QStringList& columns) { _shapeFileColumns = columns; }
  void setOgrFeatureReadLimit(const int limit) { _ogrFeatureReadLimit = limit; }
  void setConvertOps(const QStringList& ops) { _convertOps = ops; }

private:

  friend class DataConverterTest;

  QString _translationScript;
  QString _translationDirection;
  bool _translateMultithreaded;
  QStringList _shapeFileColumns;
  int _ogrFeatureReadLimit;
  QStringList _convertOps;

  Progress _progress;
  int _printLengthMax;

  void _validateInput(const QStringList& inputs, const QString& output) const;

  // converts from any input to an OGR output; A translation is required and operations are memory
  // bound.
  void _convertToOgr(const QStringList& inputs, const QString& output);
  /*
   * This method handles all non OGR output conversions.
   */
  void _convert(const QStringList& inputs, const QString& output);

  // sets ogr options only for _convert
  void _setFromOgrOptions();
  void _setToOgrOptions(const QString& output);
  void _handleNonOgrOutputTranslationOpts();
  QString _outputFormatToTranslationDirection(const QString& output) const;

  // If specific columns were specified for export to a shape file, then this is called.
  void _exportToShapeWithCols(
    const QString& output, const QStringList& cols, const OsmMapPtr& map) const;
  bool _shapeFileColumnsSpecified() const { return !_shapeFileColumns.isEmpty(); }

  // _convertToOgr will call this to run the translator in a separate thread for a performance
  // increase if certain pre-conditions are met.
  void _transToOgrMT(const QStringList& inputs, const QString& output) const;
  void _fillElementCacheMT(
    const QString& inputUrl, ElementCachePtr cachePtr, QQueue<ElementPtr>& workQ) const;
};

}

#endif // DATACONVERTER_H
