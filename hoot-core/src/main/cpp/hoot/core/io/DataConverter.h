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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DATACONVERTER_H
#define DATACONVERTER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QStringList>
#include <QQueue>
#include <QThread>
#include <QMutex>

namespace hoot
{

class elementTranslatorThread : public QThread
{
  Q_OBJECT
  void run();

public:

  QString _translation;
  QQueue<ElementPtr>* _pElementQ;
  QMutex* _pTransFeaturesQMutex;
  QMutex* _pInitMutex;
  QQueue<std::pair<boost::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrTranslator::TranslatedFeature>>>* _pTransFeaturesQ;
  bool* _pFinishedTranslating;
  ElementCachePtr _pElementCache;
};

class ogrWriterThread : public QThread
{
  Q_OBJECT
  void run();

public:

  QString _translation;
  QString _output;
  QMutex* _pTransFeaturesQMutex;
  QMutex* _pInitMutex;
  QQueue<std::pair<boost::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrTranslator::TranslatedFeature>>>* _pTransFeaturesQ;
  bool* _pFinishedTranslating;
};

/**
 * Converts data from one Hootenanny supported format to another
 */
class DataConverter : public Configurable
{

public:

  static int logWarnCount;

  DataConverter();

  virtual void setConfiguration(const Settings& conf);

  void convert(const QStringList inputs, const QString output);

  void setTranslation(const QString translation) { _translation = translation; }
  void setColumns(const QStringList columns) { _columns = columns; }
  void setColsArgSpecified(const bool specified) { _colsArgSpecified = specified; }
  void setFeatureReadLimit(const int limit) { _featureReadLimit = limit; }
  void setConvertOps(const QStringList ops) { _convertOps = ops; }

private:

  QString _translation;
  QStringList _columns;
  bool _colsArgSpecified;
  int _featureReadLimit;
  QStringList _convertOps;
  Progress _progress;

  void _validateInput(const QStringList inputs, const QString output);

  void _convertToOgr(const QString input, const QString output);
  void _convertFromOgr(const QStringList inputs, const QString output);
  void _convert(const QStringList inputs, const QString output);
  void _exportToShapeWithCols(const QString output, const QStringList cols, OsmMapPtr map);

  void _fillElementCache(QString inputUrl,
                         ElementCachePtr cachePtr,
                         QQueue<ElementPtr> &workQ);
  void _transToOgrMT(QString input, QString output);
};

}

#endif // DATACONVERTER_H
