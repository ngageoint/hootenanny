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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OGRWRITER_H
#define OGRWRITER_H

//Boost
#include <boost/shared_ptr.hpp>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/elements/ElementProvider.h>


// GDAL
#include <ogr_spatialref.h>
class OGRDataSource;
class OGRLayer;

// Qt
#include <QHash>
#include <QString>
class QString;

// Standard
#include <vector>

namespace hoot {

class Feature;
class Layer;
class Schema;

/**
 * Writes a file to an OGR data source.
 */
class OgrWriter : public PartialOsmMapWriter, public Configurable
{
public:
  static std::string className() { return "hoot::OgrWriter"; }

  /**
   * A true/false value to determine whether or not all layers are created.
   * @sa setCreateAllLayers
   */
  static QString createAllLayersKey() { return "ogr.writer.create.all.layers"; }
  /**
   * Prepends this onto each of the layer names.
   */
  static QString preLayerNameKey() { return "ogr.writer.pre.layer.name"; }
  static QString scriptKey() { return "ogr.writer.script"; }
  /**
   * Valid values are "on", "off" and "warn"
   */
  static QString strictCheckingKey() { return "ogr.strict.checking"; }

  OgrWriter();

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

  virtual ~OgrWriter();

  void close();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  virtual void setConfiguration(const Settings& conf);

  void setCreateAllLayers(bool createAll) { _createAllLayers = createAll; }

  void setPrependLayerName(const QString& pre) { _prependLayerName = pre; }

  void setScriptPath(const QString& path) { _scriptPath = path; }

  virtual void write(boost::shared_ptr<const OsmMap> map);

  virtual void finalizePartial();

  virtual void writePartial(const boost::shared_ptr<const hoot::Node>&);

  virtual void writePartial(const boost::shared_ptr<const hoot::Way>&);

  virtual void writePartial(const boost::shared_ptr<const hoot::Relation>&);

  virtual void writeElement(ElementPtr& element);

  virtual void writeElement(ElementPtr& element, bool debug);

  virtual void writeElement(ElementInputStream& inputStream);

  virtual void writeElement(ElementInputStream& inputStream, bool debug);

protected:

  bool _createAllLayers;
  bool _appendData;
  QString _scriptPath;
  mutable shared_ptr<ScriptToOgrTranslator> _translator;
  shared_ptr<OGRDataSource> _ds;
  QHash<QString, OGRLayer*> _layers;
  QString _prependLayerName;
  shared_ptr<const Schema> _schema;
  StrictChecking _strictChecking;
  bool _textStatus;
  ElementCachePtr _elementCache;
  OGRSpatialReference _wgs84;

  void _addFeature(OGRLayer* layer, shared_ptr<Feature> f, shared_ptr<Geometry> g);

  void _addFeatureToLayer(OGRLayer* layer, shared_ptr<Feature> f, const Geometry* g,
                          OGRFeature* poFeature);

  void _createLayer(shared_ptr<const Layer> layer);

  OGRLayer* _getLayer(const QString layerName);

  OGRLayer* _getLayerByName(const QString layerName);

  shared_ptr<Geometry> _toMulti(shared_ptr<Geometry> from);

  void strictError(QString warning);

  virtual void _writePartial(ElementProviderPtr& provider, const ConstElementPtr& e);

private:

  //relations that weren't written on a first pass b/c they contained relations as a member which
  //had not yet been written.
  QList<long> _unwrittenFirstPassRelationIds;
  bool _failOnSkipRelation;

};

} // hoot


#endif // OGRWRITER_H
