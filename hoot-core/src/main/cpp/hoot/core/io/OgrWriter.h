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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OGRWRITER_H
#define OGRWRITER_H

// hoot
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/io/schema/StrictChecking.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>

// GDAL
#include <ogr_spatialref.h>

// Qt
#include <QHash>
#include <QString>

// Standard
#include <vector>

namespace hoot
{

class Feature;
class Layer;
class ElementProvider;

/**
 * Writes a file to an OGR data source.
 */
class OgrWriter : public PartialOsmMapWriter, public Configurable
{
public:

  static QString className() { return "hoot::OgrWriter"; }

  OgrWriter();
  virtual ~OgrWriter() = default;

  /**
   * @brief setCacheCapacity
   *
   * @param maxNodes Number of entries for the node cache.
   * @param maxWays Number of entries for the ways cache.
   * @param maxRelations Number of entries for the relations cache.
   * @note This call deletes the existing cache and creates an entirely new one -- make sure
   *    this function is called BEFORE any data is stored in the cache
   */
  void setCacheCapacity(const unsigned long maxNodes, const unsigned long maxWays,
                        const unsigned long maxRelations);

  void close();

  virtual bool isSupported(const QString& url) override;

  // Init the translator
  void initTranslator();

  // Open our ogr output
  void openOutput(const QString& url);

  // Create layers from our translation schema
  void createAllLayers();

  void setCache(ElementCachePtr cachePtr) { _elementCache = cachePtr; }

  virtual void open(const QString& url) override;

  virtual void setConfiguration(const Settings& conf) override;

  void setCreateAllLayers(bool createAll) { _createAllLayers = createAll; }

  void setPrependLayerName(const QString& pre) { _prependLayerName = pre; }

  void setSchemaTranslationScript(const QString& path) { _scriptPath = path; }

  /**
   * @brief translateToFeatures Translates the element to a geometry and a
   *        vector of features
   * @param provider Should provide the element and all its children
   * @param e Element to translate
   * @param g Geometry output
   * @param tf Vector of translated features output
   */
  void translateToFeatures(ElementProviderPtr& provider,
                           const ConstElementPtr& e,
                           std::shared_ptr<geos::geom::Geometry>& g,
                           std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& tf);

  void writeTranslatedFeature(const std::shared_ptr<geos::geom::Geometry>& g,
                              const std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& tf);

  virtual void write(const ConstOsmMapPtr& map) override;

  virtual void finalizePartial() override;

  virtual void writePartial(const std::shared_ptr<const hoot::Node>&) override;

  virtual void writePartial(const std::shared_ptr<const hoot::Way>&) override;

  virtual void writePartial(const std::shared_ptr<const hoot::Relation>&) override;

  virtual void writeElement(ElementPtr& element) override;

  virtual void writeElement(ElementPtr& element, bool debug);

  //leaving this empty for the time being
  virtual QString supportedFormats() override { return ""; }

protected:

  static int logWarnCount;

  bool _createAllLayers;
  bool _appendData;
  QString _scriptPath;
  mutable std::shared_ptr<ScriptToOgrSchemaTranslator> _translator;
  std::shared_ptr<GDALDataset> _ds;
  /** Hash of layer names and corresponding layer objects that are owned by the GDALDataset */
  QHash<QString, OGRLayer*> _layers;
  QHash<QString, std::shared_ptr<OGRSpatialReference>> _projections;
  QString _prependLayerName;
  std::shared_ptr<const Schema> _schema;
  StrictChecking _strictChecking;
  ElementCachePtr _elementCache;
  OGRSpatialReference _wgs84;
  AddExportTagsVisitor _addExportTagsVisitor;

  void _addFeature(OGRLayer* layer, const std::shared_ptr<Feature>& f, const std::shared_ptr<geos::geom::Geometry>& g);

  void _addFeatureToLayer(OGRLayer* layer, const std::shared_ptr<Feature>& f,
                          const geos::geom::Geometry* g,
                          OGRFeature* poFeature);

  void _createLayer(const std::shared_ptr<const Layer>& layer);

  OGRLayer* _getLayer(const QString& layerName);

  OGRLayer* _getLayerByName(const QString& layerName);

  std::shared_ptr<geos::geom::Geometry> _toMulti(const std::shared_ptr<geos::geom::Geometry>& from);

  void strictError(const QString& warning);

  virtual void _writePartial(ElementProviderPtr& provider, const ConstElementPtr& e);

private:

  //relations that weren't written on a first pass b/c they contained relations as a member which
  //had not yet been written.
  QList<long> _unwrittenFirstPassRelationIds;
  bool _failOnSkipRelation;
  int _maxFieldWidth;
};

}


#endif // OGRWRITER_H
