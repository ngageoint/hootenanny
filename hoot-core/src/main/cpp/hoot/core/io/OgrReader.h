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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __OGR_READER_H__
#define __OGR_READER_H__

// Hoot
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/ProgressReporter.h>

// Qt
#include <QHash>
#include <QString>
#include <QStringList>
#include <QXmlDefaultHandler>

#include <ogr_spatialref.h>

// Standard
#include <vector>

namespace hoot
{

class ElementIterator;
class Progress;
class OgrReaderInternal;
class Settings;

/**
 * This class is broken out into an internal and external class to avoid issues with Python's
 * include file approach.
 */
class OgrReader : public PartialOsmMapReader, public ProgressReporter
{
public:

  static QString className() { return "hoot::OgrReader"; }

  /**
   * Returns true if this appears to be a reasonable path without actually attempting to open the
   * data source.
   */
  static bool isReasonablePath(const QString& path);

  OgrReader();

  OgrReader(const QString& path);

  OgrReader(const QString& path, const QString& layer);

  ~OgrReader();

  ElementIterator* createIterator(const QString& path, const QString& layer) const;

  QStringList getLayerNames(const QString& path);

  QStringList getFilteredLayerNames(const QString& path);

  /**
   * Read all geometry data from the specified path.
   *
   * @param path
   * @param layer Read only from this layer. If no layer is specified then read from all geometry
   *  layers.
   * @param map Put what we read in this map.
   */
  void read(const QString& path, const QString& layer, const OsmMapPtr& map);

  void setDefaultCircularError(Meters circularError);

  void setDefaultStatus(Status s);

  void setLimit(long limit);

  void setSchemaTranslationScript(const QString& translate);

  long getFeatureCount(const QString& path, const QString& layer);

  virtual void initializePartial() override;

  virtual bool hasMoreElements() override;

  virtual ElementPtr readNextElement() override;

  virtual void close() override;

  virtual bool isSupported(const QString& url) override;

  virtual void open(const QString& url) override;

  virtual void setUseDataSourceIds(bool useDataSourceIds) override;

  virtual void finalizePartial() override;

  /**
   * Returns the bounding box for the specified projection and configuration settings. This is
   * likely only useful in unit tests.
   */
  virtual std::shared_ptr<geos::geom::Envelope> getBoundingBoxFromConfig(const Settings& s,
    OGRSpatialReference* srs);

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const;

  //leaving this empty for the time being
  virtual QString supportedFormats() override { return ""; }

  /**
   * @see ProgressReporter
   */
  virtual void setProgress(Progress progress);
  /**
   * @see ProgressReporter
   */
  virtual unsigned int getNumSteps() const { return 1; }

protected:

  OgrReaderInternal* _d;

  Progress _progress;
};

}


#endif // __OGR_READER_H__
