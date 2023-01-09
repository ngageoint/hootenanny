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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */

#ifndef OGRWRITER_H
#define OGRWRITER_H

// hoot
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/TranslationInterface.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>

// GDAL
#include <ogr_spatialref.h>

namespace hoot
{

class Feature;
class Layer;

/**
 * Writes a file to an OGR data source.
 */
class OgrWriter : public PartialOsmMapWriter, public Configurable, public TranslationInterface, public Boundable
{
public:

  static QString className() { return "OgrWriter"; }

  OgrWriter();
  ~OgrWriter() override;

  void setConfiguration(const Settings& conf) override;

  void open(const QString& url) override;
  void write(const ConstOsmMapPtr& map) override;
  void finalizePartial() override;
  void writePartial(const ConstNodePtr& node) override;
  void writePartial(const ConstWayPtr& way) override;
  void writePartial(const ConstRelationPtr& relation) override;
  void writeElement(ElementPtr& element) override;
  void close() override;
  bool isSupported(const QString& url) const override;
  // leaving this empty for the time being
  QString supportedFormats() const override { return ""; }

  void openOutput(const QString& url);

  void createAllLayers();

  void writeTranslatedFeature(const std::shared_ptr<geos::geom::Geometry>& g,
                              const std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& tf);

  void setCreateAllLayers(bool createAll) { _createAllLayers = createAll; }
  void setPrependLayerName(const QString& pre) { _prependLayerName = pre; }
  void setCache(ElementCachePtr cachePtr) { _elementCache = cachePtr; }

protected:

  virtual void _writePartial(ElementProviderPtr& provider, const ConstElementPtr& e);

private:

  static int logWarnCount;

  bool _createAllLayers;
  bool _appendData;
  std::shared_ptr<GDALDataset> _ds;
  /* Hash of layer names and corresponding layer objects that are owned by the GDALDataset */
  QHash<QString, OGRLayer*> _layers;
  QHash<QString, std::shared_ptr<OGRSpatialReference>> _projections;
  QString _prependLayerName;
  ElementCachePtr _elementCache;
  OGRSpatialReference _wgs84;

  // contains relations that weren't written on a first pass b/c they contained relations as a
  // member which had not yet been written
  QList<long> _unwrittenFirstPassRelationIds;
  bool _failOnSkipRelation;
  int _maxFieldWidth;

  int _numWritten;
  int _transactionSize;
  bool _inTransaction;
  int _statusUpdateInterval;

  bool _forceSkipFailedRelations;

  void _addFeature(OGRLayer* layer, const std::shared_ptr<Feature>& f,
                   const std::shared_ptr<geos::geom::Geometry>& g) const;
  void _addFeatureToLayer(OGRLayer* layer, const std::shared_ptr<Feature>& f, const geos::geom::Geometry* g,
                          OGRFeature* poFeature) const;
  void _createLayer(const std::shared_ptr<const Layer>& layer);
  OGRLayer* _getLayer(const QString& layerName);
  OGRLayer* _getLayerByName(const QString& layerName);

  void _strictError(const QString& warning) const;

  bool _usesTransactions() const;
};

}


#endif // OGRWRITER_H
