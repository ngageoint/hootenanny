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

// geos
#include <ogr_spatialref.h>

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

  static QString className() { return "OgrReader"; }

  static int logWarnCount;

  OgrReader();
  OgrReader(const QString& path);
  ~OgrReader() override = default;

  void initializePartial() override;
  bool hasMoreElements() override;
  ElementPtr readNextElement() override;
  void close() override;
  bool isSupported(const QString& url) const override;
  void open(const QString& url) override;
  void setUseDataSourceIds(bool useDataSourceIds) override;
  void finalizePartial() override;

  /**
   * Read all data from the specified path.
   *
   * This is memory bound. Use the PartialOsmMapReader interface for streaming reads.
   *
   * @param path the path to the data to read
   * @param layer Read only from this layer. If no layer is specified then read from all layers.
   * @param map map to load into
   * @param jobSource optional job name for status reporting
   * @param numTasks optional number of job tasks being performed for status reporting
   */
  void read(const QString& path, const QString& layer, const OsmMapPtr& map, const QString& jobSource = "",
            const int numTasks = -1);

  /**
   * Returns true if this appears to be a reasonable path without actually attempting to open the
   * data source.
   */
  static bool isReasonablePath(const QString& path);

  long getFeatureCount(const QString& path, const QString& layer) const;
  std::shared_ptr<ElementIterator> createIterator(const QString& path, const QString& layer) const;
  /**
   * Returns a filtered list of layer names that have geometry.
   */
  QStringList getFilteredLayerNames(const QString& path) const;

  /**
   * Returns the bounding box for the specified projection and configuration settings. This is
   * likely only useful in unit tests.
   */
  virtual std::shared_ptr<geos::geom::Envelope> getBoundingBoxFromConfig(const Settings& s, const OGRSpatialReference* srs);

  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  // leaving this empty
  QString supportedFormats() const override { return ""; }

  /**
   * @see ProgressReporter
   */
  void setProgress(const Progress& progress) override;
  /**
   * @see ProgressReporter
   */
  unsigned int getNumSteps() const override { return 1; }

  void setDefaultStatus(const Status& s) override;
  void setLimit(long limit) const;
  void setSchemaTranslationScript(const QString& translate) const;

  /**
   * @brief setImportImpliedTags - Some read operations shouldn't add implied tags (i.e. tag-info)
   * @param import True for importing implied tags
   */
  void setImportImpliedTags(bool importImplied) const;

private:

  std::shared_ptr<OgrReaderInternal> _d;

  Progress _progress;

  /*
   * Attempts to determine the relative weighting of each layer in an OGR data source based on
   * feature size. If the feature size hasn't already been calculated for each layer, then a even
   * distribution of weighting between layers is returned.
   */
  std::vector<float> _getInputProgressWeights(const QString& input, const QStringList& layers) const;
  /*
   * Determines the list of layers in an OGR input. The reader must already have been initialized.
   */
  QStringList _getLayersFromPath(QString& input) const;
};

}


#endif // __OGR_READER_H__
