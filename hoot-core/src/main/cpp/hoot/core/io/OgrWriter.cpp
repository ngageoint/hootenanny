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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "OgrWriter.h"

// geos
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiPolygon.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/OgrOptions.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/schema/DoubleFieldDefinition.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/IntegerFieldDefinition.h>
#include <hoot/core/io/schema/Layer.h>
#include <hoot/core/io/schema/LongIntegerFieldDefinition.h>
#include <hoot/core/io/schema/Schema.h>
#include <hoot/core/io/schema/StringFieldDefinition.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Settings.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int OgrWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OgrWriter)

static OGRFieldType toOgrFieldType(QVariant::Type t)
{
  switch (t)
  {
  case QVariant::Int:
    return OFTInteger;
  case QVariant::LongLong:
    return OFTInteger64;
  case QVariant::String:
    return OFTString;
  case QVariant::Double:
    return OFTReal;
  default:
    throw HootException("Unsupported qvariant type: " + QString(QVariant::typeToName(t)));
  }
}

OgrWriter::OgrWriter()
  : _createAllLayers(ConfigOptions().getOgrWriterCreateAllLayers()),
    _appendData(ConfigOptions().getOgrAppendData()),
    _wgs84(),
    _failOnSkipRelation(false),
    _maxFieldWidth(-1), // We set this if we really need to.
    _numWritten(0),
    _transactionSize(ConfigOptions().getOgrWriterTransactionSize()),
    _inTransaction(false),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10),
    _cropFeaturesCrossingBounds(ConfigOptions().getWriterCropFeaturesCrossingBounds())
{
  setConfiguration(conf());

  _wgs84.SetWellKnownGeogCS("WGS84");
}

OgrWriter::~OgrWriter()
{
  //  Force the GDALDataset to be disposed before GDAL is shutdown
  close();
}

void OgrWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setCreateAllLayers(configOptions.getOgrWriterCreateAllLayers());
  setSchemaTranslationScript(configOptions.getSchemaTranslationScript());
  setPrependLayerName(configOptions.getOgrWriterPreLayerName());

  _appendData = configOptions.getOgrAppendData();
  QString strictStr = configOptions.getOgrStrictChecking();
  if (strictStr == "on")
    _strictChecking = StrictOn;
  else if (strictStr == "off")
    _strictChecking = StrictOff;
  else if (strictStr == "warn")
    _strictChecking = StrictWarn;
  else
    throw HootException("Error setting strict checking. Expected on/off/warn. got: " + strictStr);

  _statusUpdateInterval = configOptions.getTaskStatusUpdateInterval() * 10;
  _forceSkipFailedRelations = configOptions.getOgrWriterSkipFailedRelations();
  _transactionSize = configOptions.getOgrWriterTransactionSize();
  _cropFeaturesCrossingBounds = configOptions.getWriterCropFeaturesCrossingBounds();
  //  Set the bounds for cropped lines and polygons
  setBounds(Boundable::loadBounds(configOptions));
}

void OgrWriter::_strictError(const QString& warning) const
{
  if (_strictChecking == StrictOn)
    throw HootException(warning);
  else if (_strictChecking == StrictWarn)
  {
    LOG_WARN(warning);
  }
}

void OgrWriter::open(const QString& url)
{
  _numWritten = 0;

  // Initialize our translator - this will load the schema
  initTranslator();

  // Open output dataset
  openOutput(url);

  // Create all layers if _createAllLayers flag
  createAllLayers();
}

void OgrWriter::openOutput(const QString& url)
{
  bool retry = false;
  QString open_except;
  try
  {
    _ds = OgrUtilities::getInstance().openDataSource(url, false);
  }
  catch (const HootException& openException)
  {
    retry = true;
    open_except = openException.what();
  }
  if (retry)
  {
    try
    {
      _ds = OgrUtilities::getInstance().createDataSource(url);
    }
    catch (const HootException& createException)
    {
      throw HootException(QString("Error opening or creating data source. Opening error: \"%1\" "
        "Creating error: \"%2\"").arg(open_except, createException.what()));
    }
  }
  if (_usesTransactions())
    _inTransaction = _ds->StartTransaction() == OGRERR_NONE;
}

void OgrWriter::close()
{
  if (_inTransaction)
  {
    _ds->CommitTransaction();
    _inTransaction = false;
  }
  _layers.clear();
  //  Allow close() to be called multiple times without causing errors
  if (_ds)
  {
    _ds->FlushCache();
    //  Force the GDALDataset to be disposed before GDAL is shutdown
    _ds.reset();
  }
}

bool OgrWriter::isSupported(const QString& url) const
{
  LOG_VARD(_scriptPath.isEmpty());
  if (_scriptPath.isEmpty())
    return false;

  LOG_VARD(OgrUtilities::getInstance().isReasonableUrl(url));
  return OgrUtilities::getInstance().isReasonableUrl(url);
}

void OgrWriter::write(const ConstOsmMapPtr& map)
{
  _numWritten = 0;
  ElementProviderPtr provider(std::const_pointer_cast<ElementProvider>(std::dynamic_pointer_cast<const ElementProvider>(map)));

  const NodeMap& nm = map->getNodes();
  for (auto it = nm.begin(); it != nm.end(); ++it)
    _writePartial(provider, it->second);

  const WayMap& wm = map->getWays();
  for (auto it = wm.begin(); it != wm.end(); ++it)
    _writePartial(provider, it->second);

  _failOnSkipRelation = false;
  _unwrittenFirstPassRelationIds.clear();
  LOG_DEBUG("Writing first pass relations...");
  const RelationMap& rm = map->getRelations();
  for (auto it = rm.begin(); it != rm.end(); ++it)
    _writePartial(provider, it->second);

  // Since relations may contain other relations, which were unavailable to write during the first
  // pass, we're doing two write passes here.  We're only allowing two total passes for writing the
  // relations, so fail if any get skipped during the second pass.
  _failOnSkipRelation = true;
  LOG_DEBUG("Writing second pass relations...");
  for (auto relation_id : qAsConst(_unwrittenFirstPassRelationIds))
    _writePartial(provider, map->getRelation(relation_id));
}

void OgrWriter::writeTranslatedFeature(const std::shared_ptr<Geometry>& g,
                                       const vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& tf)
{
  //  Cannot write translated feature if there is no geometry
  if (!g)
    return;
  // only write the feature if it wasn't filtered by the translation script.
  for (size_t i = 0; i < tf.size(); i++)
  {
    LOG_TRACE("Writing feature " + QString::number(i) + "  to " + QString(tf[i].tableName));
    OGRLayer* layer = _getLayer(tf[i].tableName);
    if (layer != nullptr)
      _addFeature(layer, tf[i].feature, g);
  }
}

void OgrWriter::_writePartial(ElementProviderPtr& provider, const ConstElementPtr& element)
{
  std::shared_ptr<Geometry> g;
  vector<ScriptToOgrSchemaTranslator::TranslatedFeature> tf;

  ElementPtr elementClone = element->clone();
  _addExportTagsVisitor.visit(elementClone);

  translateToFeatures(provider, elementClone, g, tf);
  writeTranslatedFeature(g, tf);

  _numWritten++;
  if (_numWritten % _statusUpdateInterval == 0)
  {
    PROGRESS_STATUS(
      "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
  }
  //  Transactions should speed up the processing of larger files
  if (_inTransaction && _numWritten % _transactionSize == 0)
  {
    //  Commmit the old transaction and start a new one
    _ds->CommitTransaction();
    _inTransaction = _ds->StartTransaction() == OGRERR_NONE;
  }
}

void OgrWriter::finalizePartial()
{
  close();
}

void OgrWriter::writePartial(const ConstNodePtr& newNode)
{
  LOG_TRACE("Writing node " << newNode->getId());

  // Add to the element cache
  _addElementToCache(newNode);
  ElementProviderPtr cacheProvider = _getElementProvider();

  // It's a base datatype, so can write immediately
  _writePartial(cacheProvider, newNode);
}

void OgrWriter::writePartial(const ConstWayPtr& way)
{
  LOG_TRACE("Writing way " << way->getId() );

  /*
   * Make sure this way has any hope of working (i.e., are there enough spots in the cache
   * for all its nodes?)
   */
  if (way->getNodeCount() > getNodeCacheSize())
  {
    throw HootException(
      QString("Cannot do partial write of Way ID %1 as it contains %2 nodes, but our cache can only hold %3.  "
              "If you have enough memory to load this way, you can increase the element.cache.size.node setting to "
              "an appropriate value larger than %4 to allow for loading it.")
        .arg(QString::number(way->getId()), QString::number(way->getNodeCount()),
             QString::number(getNodeCacheSize()), QString::number(way->getNodeCount())));
  }

  // Make sure all the nodes in the way are in our cache
  const std::vector<long> wayNodeIds = way->getNodeIds();

  for (auto node_id : wayNodeIds)
  {
    if (cacheContainsElement(ElementId(ElementType::Node, node_id)) == false)
    {
      throw HootException(
        QString("Way %1 contains node %2, which is not present in the cache.  If you have the "
                "memory to support this number of nodes, you can increase the element.cache.size.node "
                "setting above: %3.")
          .arg(QString::number(way->getId()), QString::number(node_id), QString::number(getNodeCacheSize())));
    }
    LOG_TRACE("Way " << way->getId() << " contains node " << node_id <<
                 ": " << _elementCache->getNode(node_id)->getX() << ", " <<
                _elementCache->getNode(node_id)->getY() );
  }

  // Add to the element cache
  _addElementToCache(way);

  ElementProviderPtr cacheProvider = _getElementProvider();
  _writePartial(cacheProvider, way);
}

void OgrWriter::writePartial(const ConstRelationPtr& newRelation)
{
  LOG_TRACE("Writing relation " << newRelation->getId());

  // Make sure all the elements in the relation are in the cache
  const std::vector<RelationData::Entry>& relationEntries = newRelation->getMembers();
  LOG_VART(relationEntries.size());

  unsigned long nodeCount = 0;
  unsigned long wayCount = 0;
  unsigned long relationCount = 0;

  std::set<RelationData::Entry> removeEntries;

  for (const auto& member : relationEntries)
  {
    ElementType::Type member_type = member.getElementId().getType().getEnum();
    switch (member_type)
    {
    case ElementType::Node:
      nodeCount++;
      if (nodeCount > getNodeCacheSize())
      {
        throw HootException(
          QString("Relation with ID %1 contains more nodes than can fit in the cache (%2).  If you have enough memory to load this relation, "
                  "you can increase the element.cache.size.node setting to an appropriate value larger than %3 to allow for loading it.")
            .arg(QString::number(newRelation->getId()), QString::number(getNodeCacheSize()), QString::number(nodeCount)));
      }
      break;
    case ElementType::Way:
      wayCount++;
      if (wayCount > getWayCacheSize())
      {
        throw HootException(
          QString("Relation with ID %1 contains more ways than can fit in the cache (%2).  If you have enough "
                  " memory to load this relation, you can increase the element.cache.size.way setting to "
                  " an appropriate value larger than %3 to allow for loading it.")
            .arg(QString::number(newRelation->getId()), QString::number(getWayCacheSize()), QString::number(wayCount)));
      }
      break;
    case ElementType::Relation:
      relationCount++;
      if (relationCount > getRelationCacheSize())
      {
        throw HootException(
          QString("Relation with ID %1 contains more relations than can fit in the cache (%2).  If you have enough "
                  " memory to load this relation, you can increase the element.cache.size.relation setting to "
                  " to an appropriate value larger than %3 to allow for loading it.")
            .arg(QString::number(newRelation->getId()), QString::number(getRelationCacheSize()), QString::number(relationCount)));
      }
      break;
    default:
      throw HootException("Relation contains unknown type");
      break;
    }

    LOG_TRACE("Checking to see if element with ID: " << member.getElementId().getId() <<
              " and type: " << member.getElementId().getType() <<
              " contained by relation " << newRelation->getId() << " is in the element cache...");
    if (cacheContainsElement(member.getElementId()) == false )
    {
      unsigned long cacheSize;
      switch (member_type)
      {
      case ElementType::Node:
        cacheSize =  getNodeCacheSize();
        break;
      case ElementType::Way:
        cacheSize =  getWayCacheSize();
        break;
      case ElementType::Relation:
        cacheSize =  getRelationCacheSize();
        break;
      default:
        throw HootException("Relation contains unknown type");
        break;
      }
      const QString msg =
        QString("Relation (%1) element (%2: %3) does not exist in the element "
                "cache with size = %4. You may need to increase the following settings: "
                "element.cache.size.node, element.cache.size.way, element.cache.size.relation")
          .arg(QString::number(newRelation->getElementId().getId()),
               QString::number(member.getElementId().getId()),
               member.getElementId().getType().toString(),
               QString::number(cacheSize));

      if (member_type == ElementType::Relation)
      {
        //  Relation members throw that fail and aren't forced
        if (_failOnSkipRelation && !_forceSkipFailedRelations)
          throw HootException(msg);
        else if (!_failOnSkipRelation)
        {
          //  The first time through the relations, failed relation members
          //  might not have had the chance to write out to the cache yet, retry later
          LOG_TRACE(msg << " Will attempt to write relation with ID: " <<
                    newRelation->getId() << " on a subsequent pass.");
          _unwrittenFirstPassRelationIds.append(newRelation->getId());
          return;
        }
      }
      else
      {
        //  Node and Way members throw if we don't force the skip
        if (!_forceSkipFailedRelations)
          throw HootException(msg);
      }
      //  Add this member to the list to remove for OGR files
      removeEntries.insert(member);
    }
  }

  ConstElementPtr constRelation(newRelation);
  //  Update the element to remove any non-existing members
  if (!removeEntries.empty())
  {
    std::vector<RelationData::Entry> newMembers;
    for (const auto& member : newRelation->getMembers())
    {
      if (removeEntries.find(member) == removeEntries.end())
        newMembers.push_back(member);
    }
    RelationPtr p = std::dynamic_pointer_cast<Relation>(newRelation->clone());
    p->setMembers(newMembers);
    constRelation = p;
  }

  // Add to the cache
  _addElementToCache(constRelation);
  //  Write out the element
  ElementProviderPtr cacheProvider = _getElementProvider();
  _writePartial(cacheProvider, constRelation);
}

void OgrWriter::writeElement(ElementPtr& element)
{
  //  Do not attempt to write empty elements
  if (!element)
    return;
  const Tags& sourceTags = element->getTags();
  Tags destTags;
  for (auto it = sourceTags.constBegin(); it != sourceTags.constEnd(); ++it)
  {
    if (it.value() != "")
      destTags.appendValue(it.key(), it.value());
  }
  // Now that all the empties are gone, update our element
  element->setTags(destTags);

  PartialOsmMapWriter::writePartial(element);
}

void OgrWriter::createAllLayers()
{
  if (_createAllLayers)
  {
    LOG_INFO("Creating all layers...");
    for (size_t i = 0; i < _schema->getLayerCount(); ++i)
      _createLayer(_schema->getLayer(i));
  }
}

void OgrWriter::_createLayer(const std::shared_ptr<const Layer>& layer)
{
  OGRLayer *poLayer;

  OGRwkbGeometryType gtype;
  switch(layer->getGeometryType())
  {
  case GEOS_POINT:
    gtype = wkbPoint;
    break;
  case GEOS_LINESTRING:
    gtype = wkbLineString;
    break;
  case GEOS_POLYGON:
    gtype = wkbPolygon;
    break;
  default:
    throw HootException("Unexpected geometry type.");
  }

  OgrOptions options;
  if (_ds->GetDriver())
  {
    QString name = _ds->GetDriverName();
    // if this is a CSV file
    if (name == QString("CSV"))
    {
      // if we're exporting point data, then export with x/y at the front
      if (gtype == wkbPoint)
        options["GEOMETRY"] = "AS_XY";
      else // if we're exporting other geometries then export w/ WKT at the front.
        options["GEOMETRY"] = "AS_WKT";

      options["CREATE_CSVT"] = "YES";
    }

    if (name == QString("ESRI Shapefile"))
    {
      options["ENCODING"] = "UTF-8";
      _maxFieldWidth = 254; // Shapefile DBF limit
    }

    // Add a Feature Dataset to a ESRI File GeoDatabase if requested
    if (name == QString("FileGDB") && layer->getFdName() != "")
      options["FEATURE_DATASET"] = layer->getFdName();

    // So far, have only seen this needed when trying to overwrite a gpkg.
    if (name == QString("GPKG"))
      options["OVERWRITE"] = "YES";
  }

  QString layerName = _prependLayerName + layer->getName();
  poLayer = _ds->GetLayerByName(layerName.toStdString().c_str());

  // We only want to add to a layer if the config option "ogr.append.data" set.
  if (poLayer != nullptr && _appendData)
  {
    // Layer exists
    _layers[layer->getName()] = poLayer;
    // Loop through the fields making sure that they exist in the output. Print a warning if
    // they don't exist
    const OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
    std::shared_ptr<const FeatureDefinition> fd = layer->getFeatureDefinition();
    for (size_t i = 0; i < fd->getFieldCount(); i++)
    {
      std::shared_ptr<const FieldDefinition> f = fd->getFieldDefinition(i);

      if (poFDefn->GetFieldIndex(f->getName().toLatin1()) == -1)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Unable to find field: " << QString(f->getName()) << " in layer " << layerName);
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }
  else
  {
    LOG_DEBUG("Layer: " << layerName << " not found.  Creating layer...");
    std::shared_ptr<OGRSpatialReference> projection = MapProjector::createWgs84Projection();
    poLayer = _ds->CreateLayer(layerName.toLatin1(), projection.get(), gtype, options.getCrypticOptions());

    if (poLayer == nullptr)
      throw HootException(QString("Layer creation failed. %1").arg(layerName));

    _layers[layer->getName()] = poLayer;
    _projections[layer->getName()] = projection;

    std::shared_ptr<const FeatureDefinition> fd = layer->getFeatureDefinition();
    for (size_t i = 0; i < fd->getFieldCount(); i++)
    {
      std::shared_ptr<const FieldDefinition> f = fd->getFieldDefinition(i);
      OGRFieldDefn oField(f->getName().toLatin1(), toOgrFieldType(f->getType()));

      // Fix the field length but only for Strings
      if (oField.GetType() == OFTString)
      {
        // If the schema sets a field width then use it.
        if (f->getWidth() > 0)
          oField.SetWidth(f->getWidth());
        else if (_maxFieldWidth > 0) // Looking at you Shapefile.....
          oField.SetWidth(_maxFieldWidth);
      }

      int errCode = poLayer->CreateField(&oField);
      if (errCode != OGRERR_NONE)
        throw HootException(QString("Error creating field (%1)  OGR Error Code: (%2).").arg(f->getName(), QString::number(errCode)));
    }
  }
}

OGRLayer* OgrWriter::_getLayer(const QString& layerName)
{
  if (!_layers.contains(layerName))
  {
    if (!_schema->hasLayer(layerName))
    {
      _strictError("Layer specified is not part of the schema. (" + layerName + ")");
      return nullptr;
    }
    else
      _createLayer(_schema->getLayer(layerName));
  }
  return _layers[layerName];
}

void OgrWriter::_addFeature(OGRLayer* layer, const std::shared_ptr<Feature>& f, const std::shared_ptr<Geometry>& g) const
{
  OGRFeature* poFeature = OGRFeature::CreateFeature(layer->GetLayerDefn());

  // set all the column values.
  const QVariantMap& vm = f->getValues();

  for (auto it = vm.constBegin(); it != vm.constEnd(); ++it)
  {
    const QVariant& v = it.value();
    QByteArray ba = it.key().toUtf8();

    // If the field DOESN'T exist in the output layer, skip it.
    if (poFeature->GetFieldIndex(ba.constData()) == -1)
      continue;

    switch (v.type())
    {
    case QVariant::Invalid:
      poFeature->UnsetField(poFeature->GetFieldIndex(ba.constData()));
      break;
    case QVariant::Int:
      poFeature->SetField(ba.constData(), v.toInt());
      break;
    case QVariant::LongLong:
      poFeature->SetField(ba.constData(), v.toLongLong());
      break;
    case QVariant::Double:
      poFeature->SetField(ba.constData(), v.toDouble());
      break;
    case QVariant::String:
    {
      QByteArray vba = v.toString().toUtf8();

      int fieldWidth =
        poFeature->GetFieldDefnRef(poFeature->GetFieldIndex(ba.constData()))->GetWidth();

      if (vba.length() > fieldWidth && fieldWidth > 0)
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN(
            "Truncating the " << it.key() << " attribute (" << vba.length() <<
            " characters) to the output field width (" << fieldWidth << " characters).");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;

        vba.truncate(fieldWidth);
      }

      poFeature->SetField(ba.constData(), vba.constData());
      break;
    }
    default:
      _strictError("Can't convert the provided value into an OGR value. (" + v.toString() + ")");
      return;
    }
  }

  // convert the geometry.
  std::shared_ptr<GeometryCollection> gc = std::dynamic_pointer_cast<GeometryCollection>(g);
  if (gc.get() != nullptr)
  {
    for (size_t i = 0; i < gc->getNumGeometries(); i++)
    {
      const Geometry* child = gc->getGeometryN(i);
      _addFeatureToLayer(layer, f, child, poFeature);
    }
  }
  else
    _addFeatureToLayer(layer, f, g.get(), poFeature);

  OGRFeature::DestroyFeature(poFeature);
}

void OgrWriter::_addFeatureToLayer(OGRLayer* layer, const std::shared_ptr<Feature>& f,
                                   const Geometry* g, OGRFeature* poFeature) const
{
  if (g == nullptr)
    return;
  std::string wkt;
  //  Treat bounded vs unbounded differently
  if (_bounds)
  {
    if (g->intersects(_bounds.get()))
    {
      if (_cropFeaturesCrossingBounds)
      {
        //  Get the intersection of the geometry with the bounding envelope
        try
        {
          std::unique_ptr<geos::geom::Geometry> intersection = g->intersection(_bounds.get());
          wkt = intersection->toString();
        }
        catch (const geos::util::TopologyException& e)
        {
          LOG_WARN(e.what());
          return;
        }
      }
      else  //  Geometry doesn't need to be cropped
        wkt = g->toString();
    }
    else  //  Geometry is outside of the bounds, eliminate it
      return;
  }
  else  //  No bounds to change the geometry
    wkt = g->toString();
  //  Convert the WKT to an OGR geometry for saving
  const char* t = wkt.data();
  OGRGeometry* geom;
  int errCode = OGRGeometryFactory::createFromWkt(&t, layer->GetSpatialRef(), &geom) ;
  if (errCode != OGRERR_NONE)
    throw HootException(QString("Error parsing WKT (%1).  OGR Error Code: (%2)").arg(QString::fromStdString(wkt), QString::number(errCode)));

  errCode = poFeature->SetGeometryDirectly(geom);
  if (errCode != OGRERR_NONE)
    throw HootException(QString("Error setting geometry - OGR Error Code: (%1)  Geometry: (%2)").arg(QString::number(errCode), QString::fromStdString(g->toString())));

  // Unsetting the FID with SetFID(-1) before calling CreateFeature() to avoid reusing the same
  // feature object for sequential insertions
  poFeature->SetFID(-1);

  errCode = layer->CreateFeature(poFeature);
  if (errCode != OGRERR_NONE)
    throw HootException(QString("Error creating feature - OGR Error Code: (%1) \nFeature causing error: (%2)").arg(QString::number(errCode), f->toString()));
}

bool OgrWriter::_usesTransactions() const
{
  //  Right now OgrWriter will use transactions for GPKG files
  return (_ds && QString(_ds->GetDriverName()) == "GPKG");
}

}
