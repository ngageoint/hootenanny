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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OgrWriter.h"

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiPolygon.h>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
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

OgrWriter::OgrWriter()
{
  setConfiguration(conf());
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
    QByteArray ba = it.key().toAscii();

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
      QByteArray vba = v.toString().toAscii();
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

      std::string wkt = child->toString();
      char* t = (char*)wkt.data();
      OGRGeometry* geom;
      if (OGRGeometryFactory::createFromWkt(&t, layer->GetSpatialRef(), &geom) != OGRERR_NONE)
      {
        throw HootException(QString("Error parsing WKT (%1)").arg(QString::fromStdString(wkt)));
      }

      if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
      {
        throw HootException(QString("Error setting geometry"));
      }

      // reset the FID to -1 so that it will get a new FID when created.
      poFeature->SetFID(-1);
      if (layer->CreateFeature(poFeature) != OGRERR_NONE)
      {
        throw HootException(QString("Error creating feature"));
      }
    }
  }
  else
  {
    std::string wkt = g->toString();
    char* t = (char*)wkt.data();
    OGRGeometry* geom;
    if (OGRGeometryFactory::createFromWkt(&t, layer->GetSpatialRef(), &geom) != OGRERR_NONE)
    {
      throw HootException(QString("Error parsing WKT (%1)").arg(QString::fromStdString(wkt)));
    }

    if (poFeature->SetGeometryDirectly(geom) != OGRERR_NONE)
    {
      throw HootException(QString("Error setting geometry"));
    }

    if (layer->CreateFeature(poFeature) != OGRERR_NONE)
    {
      throw HootException(QString("Error creating feature"));
    }
  }

  OGRFeature::DestroyFeature(poFeature);
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

  // Check if the layer exists in the output.
  poLayer = _ds->GetLayerByName(layerName.toAscii());

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
    // Layer does not exist
    poLayer = _ds->CreateLayer(layerName.toAscii(),
                  MapReprojector::createWgs84Projection()->Clone(), gtype, options.getCrypticOptions());

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

      if( poLayer->CreateField( &oField ) != OGRERR_NONE )
      {
        throw HootException(QString("Error creating field (%1).").arg(f->getName()));
      }
    }
  } // End layer does not exist
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
  setCreateAllLayers(conf.getBool(createAllLayersKey(), false));
  setScriptPath(conf.getString(scriptKey(), ""));
  setPrependLayerName(conf.getString(preLayerNameKey(), ""));

  _appendData = ConfigOptions(conf).getOgrAppendData();

  QString strictStr = conf.getString(strictCheckingKey(), strictCheckingDefault());
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
  const OsmMap::NodeMap& nm = map->getNodeMap();
  for (OsmMap::NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    _writePartial(map, it.value());
  }

  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    _writePartial(map, it->second);
  }

  const RelationMap& rm = map->getRelationMap();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    _writePartial(map, it->second);
  }
}

void OgrWriter::_writePartial(const ConstOsmMapPtr& map, const shared_ptr<const Element>& e)
{
  if (_translator.get() == 0)
  {
    throw HootException("You must call open before attempting to write.");
  }

  if (e->getTags().getInformationCount() > 0)
  {
    shared_ptr<Geometry> g = ElementConverter(map).convertToGeometry(e);
    Tags t = e->getTags();
    t["error:circular"] = QString::number(e->getCircularError());
    t["hoot:status"] = e->getStatusString();

    // remove all the empty tags.
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

}

