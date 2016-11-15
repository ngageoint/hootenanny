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
#include "OgrWriter.h"

#include <vector>

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiPolygon.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/ElementCache.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/ScriptTranslatorFactory.h>
#include <hoot/core/io/schema/DoubleFieldDefinition.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/IntegerFieldDefinition.h>
#include <hoot/core/io/schema/Layer.h>
#include <hoot/core/io/schema/Schema.h>
#include <hoot/core/io/schema/StringFieldDefinition.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Settings.h>

#include "OgrOptions.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OgrWriter)

static OGRFieldType toOgrFieldType(QVariant::Type t)
{
  switch (t)
  {
  case QVariant::Int:
    return OFTInteger;
  case QVariant::LongLong:
    throw HootException("LongLong is not supported by OGR");
  case QVariant::String:
    return OFTString;
  case QVariant::Double:
    return OFTReal;
  default:
    throw HootException("Unsupported qvariant type: " + QString(QVariant::typeToName(t)));
  }
}

OgrWriter::OgrWriter():
  _elementCache(
    new ElementCacheLRU(
      ConfigOptions().getElementCacheSizeNode(),
      ConfigOptions().getElementCacheSizeWay(),
      ConfigOptions().getElementCacheSizeRelation())),
  _wgs84(),
  _failOnSkipRelation(false)
{
  setConfiguration(conf());

  _textStatus = ConfigOptions().getWriterTextStatus();

  _wgs84.SetWellKnownGeogCS("WGS84");
}

OgrWriter::~OgrWriter()
{

}

void OgrWriter::_addFeature(OGRLayer* layer, shared_ptr<Feature> f, shared_ptr<Geometry> g)
{
  OGRFeature* poFeature = OGRFeature::CreateFeature( layer->GetLayerDefn() );

  // set all the column values.
  const QVariantMap& vm = f->getValues();

  for (QVariantMap::const_iterator it = vm.constBegin(); it != vm.constEnd(); ++it)
  {
    const QVariant& v = it.value();
    QByteArray ba = it.key().toUtf8();

    // If the field DOESN'T exist in the output layer, skip it.
    if (poFeature->GetFieldIndex(ba.constData()) == -1)
    {
      continue;
    }

    switch (v.type())
    {
    case QVariant::Invalid:
      poFeature->UnsetField(poFeature->GetFieldIndex(ba.constData()));
      break;
    case QVariant::Int:
      poFeature->SetField(ba.constData(), v.toInt());
      break;
    case QVariant::Double:
      poFeature->SetField(ba.constData(), v.toDouble());
      break;
    case QVariant::String:
    {
      QByteArray vba = v.toString().toUtf8();
      poFeature->SetField(ba.constData(), vba.constData());
      break;
    }
    default:
      strictError("Can't convert the provided value into an OGR value. (" + v.toString() + ")");
      return;
    }
  }

  // convert the geometry.
  shared_ptr<GeometryCollection> gc = dynamic_pointer_cast<GeometryCollection>(g);
  if (gc.get() != 0)
  {
    for (size_t i = 0; i < gc->getNumGeometries(); i++)
    {
      const Geometry* child = gc->getGeometryN(i);
      _addFeatureToLayer(layer, f, child, poFeature);
    }
  }
  else
  {
    _addFeatureToLayer(layer, f, g.get(), poFeature);
  }

  OGRFeature::DestroyFeature(poFeature);
}

void OgrWriter::_addFeatureToLayer(OGRLayer* layer, shared_ptr<Feature> f, const Geometry* g,
                                   OGRFeature* poFeature)
{
  std::string wkt = g->toString();
  char* t = (char*)wkt.data();
  OGRGeometry* geom;
  int errCode = OGRGeometryFactory::createFromWkt(&t, layer->GetSpatialRef(), &geom) ;
  if (errCode != OGRERR_NONE)
  {
    throw HootException(
      QString("Error parsing WKT (%1).  OGR Error Code: (%2)").arg(QString::fromStdString(wkt)).arg(QString::number(errCode)));
  }

  errCode = poFeature->SetGeometryDirectly(geom);
  if (errCode != OGRERR_NONE)
  {
    throw HootException(
      QString("Error setting geometry - OGR Error Code: (%1)  Geometry: (%2)").arg(QString::number(errCode)).arg(QString::fromStdString(g->toString())));
  }

  //Unsetting the FID with SetFID(-1) before calling CreateFeature() to avoid reusing the same feature object for sequential insertions
  poFeature->SetFID(-1);
  errCode = layer->CreateFeature(poFeature);

  if (errCode != OGRERR_NONE)
  {
    throw HootException(
      QString("Error creating feature - OGR Error Code: (%1) \nFeature causing error: (%2)").arg(QString::number(errCode)).arg(f->toString()));
  }
}

void OgrWriter::close()
{
  _layers.clear();
  _ds.reset();
}

void OgrWriter::_createLayer(shared_ptr<const Layer> layer)
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
    // if this is a CSV file
    if (_ds->GetDriver()->GetName() == QString("CSV"))
    {
      // if we're exporting point data, then export with x/y at the front
      if (gtype == wkbPoint)
      {
        options["GEOMETRY"] = "AS_XY";
      }
      // if we're exporting other geometries then export w/ WKT at the front.
      else
      {
        options["GEOMETRY"] = "AS_WKT";
      }
      options["CREATE_CSVT"] = "YES";
    }

    if (_ds->GetDriver()->GetName() == QString("ESRI Shapefile"))
    {
      options["ENCODING"] = "UTF-8";
    }

    // Add a Feature Dataset to a ESRI File GeoDatabase if requested
    if (_ds->GetDriver()->GetName() == QString("FileGDB"))
    {
      if (layer->getFdName() != "")
      {
        options["FEATURE_DATASET"] = layer->getFdName();
        // speed up bulk inserts.
        options["FGDB_BULK_LOAD"] = "YES";
      }
    }
  }

  QString layerName = _prependLayerName + layer->getName();
  poLayer = _getLayerByName(layerName);

  // We only want to add to a layer IFF the config option "ogr.append.data" set
  if (poLayer != NULL && _appendData)
  {
    // Layer exists
    _layers[layer->getName()] = poLayer;
    // Loop through the fields making sure that they exist in the output. Print a warning if
    // they don't exist
    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

    shared_ptr<const FeatureDefinition> fd = layer->getFeatureDefinition();
    for (size_t i = 0; i < fd->getFieldCount(); i++)
    {
      shared_ptr<const FieldDefinition> f = fd->getFieldDefinition(i);

      if (poFDefn->GetFieldIndex(f->getName().toAscii()) == -1)
      {
        //        throw HootException(QString("Error: Unable to find output field: %1 in layer %2.").arg(f->getName()).arg(layerName));
        LOG_WARN("Unable to find field: " << QString(f->getName()) << " in layer " << layerName);
      }
    }
  }
  else
  {
    LOG_INFO("Layer: " << layerName << " not found.  Creating layer...");
    // Layer does not exist
    poLayer = _ds->CreateLayer(layerName.toAscii(),
                  MapProjector::createWgs84Projection()->Clone(), gtype, options.getCrypticOptions());

    if( poLayer == NULL )
    {
      throw HootException(QString("Layer creation failed. %1").arg(layerName));
    }
    _layers[layer->getName()] = poLayer;

    shared_ptr<const FeatureDefinition> fd = layer->getFeatureDefinition();
    for (size_t i = 0; i < fd->getFieldCount(); i++)
    {
      shared_ptr<const FieldDefinition> f = fd->getFieldDefinition(i);
      OGRFieldDefn oField(f->getName().toAscii(), toOgrFieldType(f->getType()));
      if (f->getWidth() > 0)
      {
        oField.SetWidth(f->getWidth());
      }

      int errCode = poLayer->CreateField(&oField);
      if (errCode != OGRERR_NONE)
      {
        throw HootException(
          QString("Error creating field (%1)  OGR Error Code: (%2).").arg(f->getName()).arg(QString::number(errCode)));
      }
    }
  } // End layer does not exist
}

OGRLayer* OgrWriter::_getLayerByName(const QString layerName)
{
  // Check if the layer exists in the output.
  int layerCount = _ds->GetLayerCount();
  for (int i = 0; i < layerCount; i++)
  {
    OGRLayer* layer = _ds->GetLayer(i+1);
    if (layer != NULL)
    {
      QString tmpLayerName = QString(layer->GetName());
      if (tmpLayerName == layerName)
      {
        return layer;
      }
    }
  }
  return NULL;
}

OGRLayer* OgrWriter::_getLayer(const QString layerName)
{
  if (!_layers.contains(layerName))
  {
    bool found = false;
    for (size_t i = 0; i < _schema->getLayerCount(); ++i)
    {
      if (_schema->getLayer(i)->getName() == layerName)
      {
        _createLayer(_schema->getLayer(i));
        found = true;
      }
    }

    if (!found)
    {
      strictError("Layer specified is not part of the schema. (" + layerName + ")");
      return 0;
    }
  }
  return _layers[layerName];
}

bool OgrWriter::isSupported(QString url)
{
  if (_scriptPath.isEmpty())
  {
    return false;
  }
  return OgrUtilities::getInstance().isReasonableUrl(url);
}

void OgrWriter::open(QString url)
{
  if (_scriptPath.isEmpty())
  {
    throw HootException("A script path must be set before the output data source is opened.");
  }

  if (_translator == 0)
  {
    // Great bit of code taken from TranslatedTagDifferencer.cpp
    shared_ptr<ScriptTranslator> st(ScriptTranslatorFactory::getInstance().createTranslator(
         _scriptPath));
    st->setErrorTreatment(_strictChecking);
    _translator = dynamic_pointer_cast<ScriptToOgrTranslator>(st);
  }

  if (!_translator)
  {
    throw HootException("Error allocating translator, the translation script must support "
                        "converting to OGR.");
  }

  _schema = _translator->getOgrOutputSchema();

  try
  {
    _ds = OgrUtilities::getInstance().openDataSource(url);
  }
  catch(HootException& openException)
  {
    try
    {
      _ds = OgrUtilities::getInstance().createDataSource(url);
    }
    catch(HootException& createException)
    {
      throw HootException(QString("Error opening or creating data source. Opening error: \"%1\" "
        "Creating error: \"%2\"").arg(openException.what()).arg(createException.what()));
    }
  }

  if (_createAllLayers)
  {
    for (size_t i = 0; i < _schema->getLayerCount(); ++i)
    {
      _createLayer(_schema->getLayer(i));
    }
  }
}

void OgrWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setCreateAllLayers(configOptions.getOgrWriterCreateAllLayers());
  setScriptPath(configOptions.getOgrWriterScript());
  setPrependLayerName(configOptions.getOgrWriterPreLayerName());

  _appendData = configOptions.getOgrAppendData();

  QString strictStr = configOptions.getOgrStrictChecking();
  if (strictStr == "on")
  {
    _strictChecking = StrictOn;
  }
  else if (strictStr == "off")
  {
    _strictChecking = StrictOff;
  }
  else if (strictStr == "warn")
  {
    _strictChecking = StrictWarn;
  }
  else
  {
    throw HootException("Error setting strict checking. Expected on/off/warn. got: " + strictStr);
  }
}

shared_ptr<Geometry> OgrWriter::_toMulti(shared_ptr<Geometry> from)
{
  shared_ptr<Geometry> result;

  switch (from->getGeometryTypeId())
  {
  case GEOS_POINT:
  {
    vector<Geometry*> v;
    v.push_back(from.get());
    result.reset(GeometryFactory::getDefaultInstance()->createMultiPoint(v));
    break;
  }
  case GEOS_LINESTRING:
  {
    vector<Geometry*> v;
    v.push_back(from.get());
    result.reset(GeometryFactory::getDefaultInstance()->createMultiLineString(v));
    break;
  }
  case GEOS_POLYGON:
  {
    vector<Geometry*> v;
    v.push_back(from.get());
    result.reset(GeometryFactory::getDefaultInstance()->createMultiPolygon(v));
    break;
  }
  case GEOS_MULTIPOINT:
  case GEOS_MULTILINESTRING:
  case GEOS_MULTIPOLYGON:
    result = from;
    break;
  default:
    throw HootException("Unexpected geometry type: " + from->getGeometryType());
  }

  return result;
}

void OgrWriter::strictError(QString warning)
{
  if (_strictChecking == StrictOn)
  {
    throw HootException(warning);
  }
  else if (_strictChecking == StrictWarn)
  {
    LOG_WARN(warning);
  }
}

void OgrWriter::write(shared_ptr<const OsmMap> map)
{
  ElementProviderPtr provider(boost::const_pointer_cast<ElementProvider>(
    boost::dynamic_pointer_cast<const ElementProvider>(map)));

  const NodeMap& nm = map->getNodeMap();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    _writePartial(provider, it->second);
  }

  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    _writePartial(provider, it->second);
  }

  _failOnSkipRelation = false;
  _unwrittenFirstPassRelationIds.clear();
  LOG_DEBUG("Writing first pass relations...");
  const RelationMap& rm = map->getRelationMap();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    _writePartial(provider, it->second);
  }
  //Since relations may contain other relations, which were unavailable to write during the first
  //pass, we're doing two write passes here.  We're only allowing two total passes for writing the
  //relations, so fail if any get skipped during the second pass.
  _failOnSkipRelation = true;
  LOG_DEBUG("Writing second pass relations...");
  for (QList<long>::const_iterator relationIdIter = _unwrittenFirstPassRelationIds.begin();
       relationIdIter != _unwrittenFirstPassRelationIds.end(); relationIdIter++)
  {
    _writePartial(provider, map->getRelation(*relationIdIter));
  }
}

void OgrWriter::_writePartial(ElementProviderPtr& provider, const ConstElementPtr& e)
{
  if (_translator.get() == 0)
  {
    throw HootException("You must call open before attempting to write.");
  }

  if (e->getTags().getInformationCount() > 0)
  {
    // There is probably a cleaner way of doing this.
    // convertToGeometry calls  getGeometryType which will throw an exception if it gets a relation
    // that it doesn't know about. E.g. "route", "superroute", " turnlanes:turns" etc

    shared_ptr<Geometry> g;

    try
    {
      g = ElementConverter(provider).convertToGeometry(e);
    }
    catch (IllegalArgumentException& err)
    {
      LOG_WARN("Error converting geometry: " << err.getWhat() << " (" << e->toString() << ")");
      g.reset((GeometryFactory::getDefaultInstance()->createEmptyGeometry()));
    }

    /*LOG_DEBUG("After conversion to geometry, element is now a " <<
             g->getGeometryType() );*/

    Tags t = e->getTags();
    t["error:circular"] = QString::number(e->getCircularError());

    if (_textStatus)
    {
      t["hoot:status"] = e->getStatus().toTextStatus();
    }
    else
    {
      t["hoot:status"] = e->getStatusString();
    }

    for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
    {
      if (t[it.key()] == "")
      {
        t.remove(it.key());
      }
    }

    vector<ScriptToOgrTranslator::TranslatedFeature> tf = _translator->translateToOgr(t,
      e->getElementType(), g->getGeometryTypeId());

    // only write the feature if it wasn't filtered by the translation script.
    for (size_t i = 0; i < tf.size(); i++)
    {
      OGRLayer* layer = _getLayer(tf[i].tableName);
      if (layer != 0)
      {
        _addFeature(layer, tf[i].feature, g);
      }
    }
  }
}

void OgrWriter::finalizePartial()
{
  /// @todo implement
}

void OgrWriter::writePartial(const boost::shared_ptr<const hoot::Node>& newNode)
{
  LOG_DEBUG("Writing node " << newNode->getId());

  // Add to the element cache
  ConstElementPtr myNode(newNode);
  _elementCache->addElement(myNode);
  ElementProviderPtr cacheProvider(_elementCache);

  // It's a base datatype, so can write immediately
  _writePartial(cacheProvider, newNode);
}

void OgrWriter::writePartial(const boost::shared_ptr<const hoot::Way>& newWay)
{
  LOG_DEBUG("Writing way " << newWay->getId() );

  /*
   * Make sure this way has any hope of working (i.e., are there enough spots in the cache
   * for all its nodes?
   */
  if ((unsigned long)newWay->getNodeCount() > _elementCache->getNodeCacheSize())
  {
    throw HootException("Cannot do partial write of Way ID " + QString::number(newWay->getId()) +
      " as it contains " + QString::number(newWay->getNodeCount()) + " nodes, but our cache can " +
      " only hold " + QString::number(_elementCache->getNodeCacheSize()) + ".  If you have enough " +
      " memory to load this way, you can increase the element.cache.size.node setting to " +
      " an appropriate value larger than " + QString::number(newWay->getNodeCount()) +
      " to allow for loading it.");
  }

  // Make sure all the nodes in the way are in our cache
  const std::vector<long> wayNodeIds = newWay->getNodeIds();
  std::vector<long>::const_iterator nodeIdIterator;

  for (nodeIdIterator = wayNodeIds.begin(); nodeIdIterator != wayNodeIds.end(); nodeIdIterator++)
  {
    if (_elementCache->containsNode(*nodeIdIterator) == false)
    {
      throw HootException("Way " + QString::number(newWay->getId()) + " contains node " +
        QString::number(*nodeIdIterator) + ", which is not present in the cache.  If you have the " +
          "memory to support this number of nodes, you can increase the element.cache.size.node " +
          "setting above: " + QString::number(_elementCache->getNodeCacheSize()) + ".");
    }
    LOG_DEBUG("Way " << newWay->getId() << " contains node " << *nodeIdIterator <<
                 ": " << _elementCache->getNode(*nodeIdIterator)->getX() << ", " <<
                _elementCache->getNode(*nodeIdIterator)->getY() );
  }

  // Add to the element cache
  ConstElementPtr constWay(newWay);
  _elementCache->addElement(constWay);

  ElementProviderPtr cacheProvider(_elementCache);
  _writePartial(cacheProvider, newWay);
}

void OgrWriter::writePartial(const boost::shared_ptr<const hoot::Relation>& newRelation)
{
  LOG_DEBUG("Writing relation " << newRelation->getId());

  // Make sure all the elements in the relation are in the cache
  const std::vector<RelationData::Entry>& relationEntries = newRelation->getMembers();
  LOG_VARD(relationEntries.size());

  unsigned long nodeCount = 0;
  unsigned long wayCount = 0;
  unsigned long relationCount = 0;

  for (std::vector<RelationData::Entry>::const_iterator relationElementIter = relationEntries.begin();
       relationElementIter != relationEntries.end(); relationElementIter++)
  {
    switch (relationElementIter->getElementId().getType().getEnum())
    {
      case ElementType::Node:
        nodeCount++;
        if (nodeCount > _elementCache->getNodeCacheSize())
        {
          throw HootException("Relation with ID " +
            QString::number(newRelation->getId()) + " contains more nodes than can fit in the " +
            "cache (" + QString::number(_elementCache->getNodeCacheSize()) + ").  If you have enough " +
            " memory to load this relation, you can increase the element.cache.size.node setting to " +
            " an appropriate value larger than " + QString::number(nodeCount) + " to allow for loading it.");
        }
        break;
      case ElementType::Way:
        wayCount++;
        if (wayCount > _elementCache->getWayCacheSize())
        {
          throw HootException("Relation with ID " +
            QString::number(newRelation->getId()) + " contains more ways than can fit in the " +
            "cache (" + QString::number(_elementCache->getWayCacheSize()) + ").  If you have enough " +
            " memory to load this relation, you can increase the element.cache.size.way setting to " +
            " an appropriate value larger than " + QString::number(wayCount) +
            " to allow for loading it.");
        }

        break;
      case ElementType::Relation:
        relationCount++;
        if (relationCount > _elementCache->getRelationCacheSize())
        {
          throw HootException("Relation with ID " +
            QString::number(newRelation->getId()) + " contains more relations than can fit in the " +
            "cache (" + QString::number(_elementCache->getRelationCacheSize()) + ").  If you have enough " +
            " memory to load this relation, you can increase the element.cache.size.relation setting to " +
            " to an appropriate value larger than " + QString::number(relationCount) +
            " to allow for loading it.");
        }

        break;
      default:
        throw HootException("Relation contains unknown type");
        break;
    }

    LOG_DEBUG("Checking to see if element with ID: " << relationElementIter->getElementId().getId() <<
              " and type: " << relationElementIter->getElementId().getType() <<
              " contained by relation " << newRelation->getId() << " is in the element cache...");
    if ( _elementCache->containsElement(relationElementIter->getElementId()) == false )
    {
      unsigned long cacheSize;
      switch (relationElementIter->getElementId().getType().getEnum())
      {
        case ElementType::Node:
          cacheSize =  _elementCache->getNodeCacheSize();
          break;
        case ElementType::Way:
          cacheSize =  _elementCache->getWayCacheSize();
          break;
        case ElementType::Relation:
          cacheSize =  _elementCache->getRelationCacheSize();
          break;
        default:
          throw HootException("Relation contains unknown type");
          break;
      }
      const QString msg = "Relation element with ID: " +
        QString::number(relationElementIter->getElementId().getId()) + " and type: " +
        relationElementIter->getElementId().getType().toString() + " did not exist in the element " +
        "cache with size = " + QString::number(cacheSize) + ".  You may need to increase the following " +
        "settings: element.cache.size.node, element.cache.size.way, element.cache.size.relation";
      if (_failOnSkipRelation ||
          relationElementIter->getElementId().getType().getEnum() != ElementType::Relation)
      {
        throw HootException(msg);
      }
      else
      {
        LOG_DEBUG(msg << "   Will attempt to write relation with ID: " + newRelation->getId() <<
                 " on a subsequent pass.");
        _unwrittenFirstPassRelationIds.append(newRelation->getId());
        return;
      }
    }
  }

  // Add to the cache
  ConstElementPtr constRelation(newRelation);
  _elementCache->addElement(constRelation);

  ElementProviderPtr cacheProvider(_elementCache);
    _writePartial(cacheProvider, newRelation);
}

void OgrWriter::writeElement(ElementPtr &element)
{
  writeElement(element, false);
}

void OgrWriter::writeElement(ElementInputStream& inputStream)
{
  writeElement(inputStream, false);
}

void OgrWriter::writeElement(ElementInputStream& inputStream, bool debug)
{
  // Make sure incoming element is in WGS84
  assert( inputStream.getProjection()->IsSame(&_wgs84) == true );
  ElementPtr nextElement = inputStream.readNextElement();

  writeElement(nextElement, debug);
}

void OgrWriter::writeElement(ElementPtr &element, bool debug)
{
  Tags sourceTags = element->getTags();
  Tags destTags;
  for (Tags::const_iterator it = element->getTags().begin();
       it != element->getTags().end(); ++it)
  {
    if (sourceTags[it.key()] != "")
    {
      destTags.appendValue(it.key(), it.value());
    }
  }
  // Now that all the empties are gone, update our element
  element->setTags(destTags);

  if ( debug == true )
  {
    LOG_DEBUG(element->toString());
  }

  PartialOsmMapWriter::writePartial(element);
}

void OgrWriter::setCacheCapacity(const unsigned long maxNodes, const unsigned long maxWays,
                                 const unsigned long maxRelations)
{
  _elementCache.reset();
  _elementCache =
    boost::shared_ptr<ElementCache>(new ElementCacheLRU(maxNodes, maxWays, maxRelations));
}

}
