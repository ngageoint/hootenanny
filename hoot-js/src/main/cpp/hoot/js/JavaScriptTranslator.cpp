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

#include "JavaScriptTranslator.h"

// hoot
#include <hoot/core/Exception.h>
#include <hoot/core/Factory.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/DoubleFieldDefinition.h>
#include <hoot/core/io/schema/IntegerFieldDefinition.h>
#include <hoot/core/io/schema/StringFieldDefinition.h>
#include <hoot/core/io/schema/Layer.h>
#include <hoot/core/io/schema/Schema.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/PluginContext.h>

// Qt
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>

// TBS
#include <tbs/stats/SampleStats.h>

// Tgs
#include <tgs/System/Time.h>

Q_DECLARE_METATYPE(hoot::JavaScriptTranslator*)
using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(ScriptTranslator, JavaScriptTranslator)

// Return the current time
Handle<Value> jsGetTimeNow(const Arguments& /*args*/)
{
  return toV8(Tgs::Time::getTime());
}

JavaScriptTranslator::JavaScriptTranslator()
{
  setConfiguration(conf());
}

void JavaScriptTranslator::setConfiguration(const Settings& conf)
{
  _conf = conf;
}

JavaScriptTranslator::~JavaScriptTranslator()
{
  if (_timing.size() != 0)
  {
    tbs::SampleStats stats(_timing);
    LOG_DEBUG("Translation script run time (ms): " << stats.toString());
  }
  close();
}

vector<JavaScriptTranslator::TranslatedFeature> JavaScriptTranslator::_createAllFeatures(
  QVariantList list)
{
  vector<TranslatedFeature> result;
  result.reserve(list.size());

  for (int i = 0; i < list.size(); i++)
  {
    TranslatedFeature tf;
    tf.feature = _createFeature(list[i].toMap(), tf.tableName);
    result.push_back(tf);
  }

  return result;
}

shared_ptr<Feature> JavaScriptTranslator::_createFeature(QVariantMap vm, QString &tableName)
{
  if (vm.contains("attrs") == false)
  {
    throw HootException("Expected translateToOgr result to contain 'attrs' property.");
  }
  if (vm.contains("tableName") == false)
  {
    throw HootException("Expected translateToOgr result to contain 'tableName' property.");
  }

  tableName = vm["tableName"].toString();

  // figure out which layer this feature belongs in.
  shared_ptr<const Layer> layer;

  for (size_t i = 0; i < _schema->getLayerCount(); ++i)
  {
    shared_ptr<const Layer> l = _schema->getLayer(i);

    if (l->getName() == tableName)
    {
      layer = l;
    }
  }

  if (layer == 0)
  {
    strictError("Table name: " + tableName + " not found in schema.");
  }

  // copy the attributes into the feature.
  if (vm["attrs"].canConvert(QVariant::Map) == false)
  {
    throw HootException("Expected attrs to be an object.");
  }

  QVariantMap attrs = vm["attrs"].toMap();

  shared_ptr<Feature> result(new Feature(layer->getFeatureDefinition()));


  for (QVariantMap::const_iterator it = attrs.begin(); it != attrs.end(); ++it)
  {
    if (result->getFeatureDefinition()->hasField(it.key()) == false)
    {
      QString error("The specified field/column is not part of this table definition."
                    " table: " + tableName + " column: " + it.key());

      if (_strict == StrictOn)
      {
        throw FieldDefinition::InvalidValueException(it.key(), error);
      }
      else
      {
        LOG_WARN(error);
      }
    }
    else
    {
      result->setField(it.key(), it.value());
    }
  }

  result->validate(_strict);

  return result;
}

void JavaScriptTranslator::_finalize()
{
  // if we didn't error out.
  if (!_error)
  {
    // run the user's finalize function if it exists.
    HandleScope handleScope;
    Context::Scope context_scope(_gContext->getContext());

    Handle<Object> tObj = _gContext->getContext()->Global();

    if (tObj->Has(String::NewSymbol("finalize")))
    {
      TryCatch trycatch;
      Handle<Value> final = _gContext->call(tObj,"finalize");
      HootExceptionJs::checkV8Exception(final, trycatch);
    }
  }

  _initialized = false;
}

int JavaScriptTranslator::getLogCount(QString log)
{
  int result;

  if (_logs.contains(log) == false)
  {
    _logs[log] = 1;
    result = 1;
  }
  else
  {
    result = _logs[log] + 1;
    _logs[log] = result;
  }

  return result;
}


void JavaScriptTranslator::_init()
{
  LOG_DEBUG("Loading script: " << _scriptPath);

  _error = false;
  _gContext.reset(new PluginContext());
  HandleScope handleScope;
  Context::Scope context_scope(_gContext->getContext());

  if (_scriptPath.isEmpty())
  {
    if (_scriptText.isEmpty())
    {
      throw HootException("Please either specify a script path or text.");
    }
    else
    {
      _gContext->loadText(_scriptText);
    }
  }
  else
  {
    _gContext->loadScript(_scriptPath);
  }

  // Less typeing
  Handle<Object> tObj = _gContext->getContext()->Global();

  // Set up a small function
  tObj->Set(String::NewSymbol("timeNow"), FunctionTemplate::New(jsGetTimeNow)->GetFunction());


  // Run Initiallise, if it exists
  if (tObj->Has(String::NewSymbol("initialize")))
  {
    TryCatch trycatch;
    Handle<Value> initial = _gContext->call(tObj,"initialize");
    HootExceptionJs::checkV8Exception(initial, trycatch);
  }

  // Sort out what the toOsm function is called
  if (tObj->Has(String::NewSymbol("translateToOsm")))
  {
    _toOsmFunctionName = "translateToOsm";
  }
  else if (tObj->Has(String::NewSymbol("translateAttributes")))
  {
    _toOsmFunctionName = "translateAttributes";
  }
  else
  {
    throw HootException("A 'translateToOsm' function must be defined.");
  }

  // Debug Stuff - Dump the object properties
//  Handle<Object> hoot = tObj->Get(toV8("hoot"))->ToObject();

//  cout << endl;
//  cout << "JSTrans:" << endl;
//  cout << "tObj Properties: " << tObj->GetPropertyNames() << endl;
//  cout << endl;
//  cout << "Hoot Properties: " << hoot->GetPropertyNames() << endl;
//  cout << endl;
//  cout << "End: JSTrans:" << endl;
//  cout << endl;

  LOG_DEBUG("Script loaded.");
  _initialized = true;
}



// Use the layerNameFilter function to get the filter string (regexp)
const QString JavaScriptTranslator::getLayerNameFilter()
{
  // Just making sure
  if (!_initialized)
  {
    _init();
  }

  HandleScope handleScope;
  Context::Scope context_scope(_gContext->getContext());

  Handle<Object> tObj = _gContext->getContext()->Global();

  if (tObj->Has(String::NewSymbol("layerNameFilter")))
  {
    return toCpp<QString>(_gContext->call(tObj,"layerNameFilter"));
  }
  else
  {
    return ScriptTranslator::getLayerNameFilter();
  }
}

QVariant& JavaScriptTranslator::_getMapValue(QVariantMap& map, QString key)
{
  if (map.contains(key) == false)
  {
    throw HootException("Expected to find key: " + key);
  }
  return map[key];
}

bool JavaScriptTranslator::isValidScript()
{
  bool result = false;

  QFileInfo info(_scriptPath);

  if (_scriptPath.endsWith(".js", Qt::CaseInsensitive) && info.exists())
  {
    if (!_initialized)
    {
      try
      {
        _init();
        result = true;
      }
      catch (const HootException& e)
      {
        LOG_ERROR("Error initializing JavaScript: " +  e.getWhat());
        result = false;
      }
    }
    else
    {
      result = true;
    }
  }
  return result;
}

void JavaScriptTranslator::_featureWarn(QString message, QString fileName, QString functionName,
  int lineNumber)
{
//  stringstream ss;
//  LOG_INFO(_tags);
//  ss << message << " Input tags: " << *_tags;
  Log::getInstance().log(Log::Warn, message.toStdString(), fileName.toStdString(),
                         functionName.toStdString(), lineNumber);
}

shared_ptr<const Schema> JavaScriptTranslator::getOgrOutputSchema()
{
  LOG_DEBUG("Started getOgrOutputSchema");

  if (_schema == 0)
  {
    if (!_initialized)
    {
      _init();
    }

    HandleScope handleScope;
    Context::Scope context_scope(_gContext->getContext());

    Handle<Object> tObj = _gContext->getContext()->Global();

    if (!tObj->Has(String::NewSymbol("getDbSchema")))
    {
      throw HootException("This translation file does not support converting to OGR. "
                          "(Missing schema)");
    }

    Handle<Value> schemaJs(_gContext->call(tObj,"getDbSchema"));

    if (schemaJs->IsArray())
    {

      shared_ptr<Schema> schema(new Schema());
      QVariantList schemaV = toCpp<QVariant>(schemaJs).toList();

      for (int i = 0; i < schemaV.size(); ++i)
      {
        schema->addLayer(_parseLayer(schemaV[i]));
      }

      _schema = schema;
    }
    else
    {
      throw Exception("Expected a valid schema array to be returned.");
    }
  }

//  LOG_WARN("Returning from GetOgrOutputSchema")
//  shared_ptr<Schema> schema(new Schema());

  return _schema;
}

void JavaScriptTranslator::_parseEnumerations(DoubleFieldDefinition* fd, QVariant& enumerations)
  const
{
  if (enumerations.canConvert(QVariant::List) == false)
  {
    throw HootException("Expected enumerations to be an array of maps.");
  }
  QVariantList vl = enumerations.toList();

  for (int i = 0; i < vl.size(); i++)
  {
    if (vl[i].canConvert(QVariant::Map) == false)
    {
      throw HootException("Expected enumerations to be an array of maps.");
    }
    QVariantMap vm = vl[i].toMap();

    if (vm["value"].canConvert(QVariant::Double) == false)
    {
      throw HootException("Expected each enumeration map to contain a valid value.");
    }
    double v = vm["value"].toDouble();

    if (fd->hasEnumeratedValue(v))
    {
      LOG_WARN("Enumerated value repeated in enumerations table: " << v);
    }
    else
    {
      fd->addEnumeratedValue(v);
    }
  }
}

void JavaScriptTranslator::_parseEnumerations(IntegerFieldDefinition* fd, QVariant& enumerations)
  const
{
  if (enumerations.canConvert(QVariant::List) == false)
  {
    throw HootException("Expected enumerations to be an array of maps.");
  }
  QVariantList vl = enumerations.toList();

  for (int i = 0; i < vl.size(); i++)
  {
    if (vl[i].canConvert(QVariant::Map) == false)
    {
      throw HootException("Expected enumerations to be an array of maps.");
    }
    QVariantMap vm = vl[i].toMap();

    if (vm["value"].canConvert(QVariant::Int) == false)
    {
      throw HootException("Expected each enumeration map to contain a valid value.");
    }
    int v = vm["value"].toInt();

    if (fd->hasEnumeratedValue(v))
    {
      LOG_WARN("Enumerated value repeated in enumerations table: " << v);
    }
    else
    {
      fd->addEnumeratedValue(v);
    }
  }
}

shared_ptr<FieldDefinition> JavaScriptTranslator::_parseFieldDefinition(QVariant fieldV) const
{
  shared_ptr<FieldDefinition> result;

  if (fieldV.canConvert(QVariant::Map) == false)
  {
    throw HootException("Expected the column to be an object (map).");
  }

  QVariantMap map = fieldV.toMap();

  if (map.contains("type") == false)
  {
    throw HootException("Error parsing type in column.");
  }
  QString type = map["type"].toString().toLower();
  if (type == "string")
  {
    StringFieldDefinition* fd = new StringFieldDefinition();
    result.reset(fd);

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
      {
        fd->setDefaultIsNull(true);
      }
      else
      {
        fd->setDefaultValue(map["defValue"].toString());
      }
    }
    if (map.contains("length"))
    {
      fd->setWidth(_toInt32(map["length"]));
    }
  }
  else if (type == "double" || type == "real")
  {
    DoubleFieldDefinition* fd = new DoubleFieldDefinition();
    result.reset(fd);

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
      {
        fd->setDefaultIsNull(true);
      }
      else
      {
        fd->setDefaultValue(_toDouble(map["defValue"]));
      }
    }
    if (map.contains("maximum"))
    {
      fd->setMaxValue(_toDouble(map["maximum"]));
    }
    if (map.contains("minimum"))
    {
      fd->setMinValue(_toDouble(map["minimum"]));
    }

    if (map.contains("enumerations"))
    {
      _parseEnumerations(fd, map["enumerations"]);
    }
  }
  else if (type == "enumeration" || type == "long integer" || type == "integer")
  {
    IntegerFieldDefinition* fd = new IntegerFieldDefinition();
    result.reset(fd);

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
      {
        fd->setDefaultIsNull(true);
      }
      else
      {
        fd->setDefaultValue(_toInt32(map["defValue"]));
      }
    }
    if (map.contains("maximum"))
    {
      fd->setMaxValue(_toInt32(map["maximum"]));
    }
    if (map.contains("minimum"))
    {
      fd->setMinValue(_toInt32(map["minimum"]));
    }

    if (map.contains("enumerations"))
    {
      _parseEnumerations(fd, map["enumerations"]);
    }
  }
  else
  {
    throw HootException("Unexpected column type: " + type);
  }

  if (map.contains("name") == false)
  {
    throw HootException("Error parsing name in column.");
  }
  result->setName(map["name"].toString());
  if (map.contains("allowNull"))
  {
    QString s = map["allowNull"].toString().toLower();
    if (s == "true")
    {
      result->setAllowNull(true);
    }
    else if (s == "false")
    {
      result->setAllowNull(false);
    }
    else
    {
      throw HootException("Expected a value of 'true' or 'false' for allowNull.");
    }
  }

  return result;
}

shared_ptr<Layer> JavaScriptTranslator::_parseLayer(QVariant layer) const
{
  shared_ptr<Layer> newLayer(new Layer());

  if (layer.canConvert(QVariant::Map) == false)
  {
    throw HootException("Expected the layer to be an object (map).");
  }

  QVariantMap map = layer.toMap();

  // parse out the name of the layer.
  if (map.contains("name") == false)
  {
    throw HootException("Expected layer to contain a name.");
  }
  newLayer->setName(map["name"].toString());

  // parse out the geometry portion of the layer.
  if (map.contains("geom") == false)
  {
    throw HootException(QString("Expected layer (%1) to contain a geom.").arg(newLayer->getName()));
  }
  QString geom = map["geom"].toString().toLower();
  if (geom == "line")
  {
    newLayer->setGeometryType(GEOS_LINESTRING);
  }
  else if (geom == "point")
  {
    newLayer->setGeometryType(GEOS_POINT);
  }
  else if (geom == "area" || geom == "polygon")
  {
    newLayer->setGeometryType(GEOS_POLYGON);
  }
  else
  {
    throw HootException(QString("Expected layer (%1) to contain a valid geom.").
      arg(newLayer->getName()));
  }

  if (map.contains("fdname") == true)
  {
    newLayer->setFdName(map["fdname"].toString());
  }

  // parse out the columns one at a time.
  QVariant columnsV = map["columns"];
  if (columnsV.canConvert(QVariant::List) == false)
  {
    throw HootException(QString("Expected layer (%1) to contain a list of columns.").
      arg(newLayer->getName()));
  }
  set<QString> names;
  QVariantList columns = columnsV.toList();
  shared_ptr<FeatureDefinition> dfd(new FeatureDefinition());
  for (int i = 0; i < columns.size(); i++)
  {
    shared_ptr<FieldDefinition> fd = _parseFieldDefinition(columns[i]);
    if (names.find(fd->getName()) != names.end())
    {
      throw HootException("Found multiple fields with the same name. (" + fd->getName() + ")");
    }
    dfd->addField(fd);
  }
  newLayer->setFeatureDefinition(dfd);

  return newLayer;
}

qint32 JavaScriptTranslator::_toInt32(const QVariant& v) const
{
  if (v.canConvert(QVariant::Int) == false)
  {
    throw HootException("Error parsing int: " + v.toString());
  }
  return v.toInt();
}

qint64 JavaScriptTranslator::_toInt64(const QVariant& v) const
{
  if (v.canConvert(QVariant::LongLong) == false)
  {
    throw HootException("Error parsing long int: " + v.toString());
  }
  return v.toLongLong();
}

double JavaScriptTranslator::_toDouble(const QVariant& v) const
{
  if (v.canConvert(QVariant::Double) == false)
  {
    throw HootException("Error parsing double: " + v.toString());
  }
  return v.toDouble();
}

vector<ScriptToOgrTranslator::TranslatedFeature> JavaScriptTranslator::translateToOgr(Tags& tags,
  ElementType elementType, geos::geom::GeometryTypeId geometryType)
{
  return _createAllFeatures(_translateToOgrVariants(tags, elementType, geometryType));
}

vector<Tags> JavaScriptTranslator::translateToOgrTags(Tags& tags, ElementType elementType,
  GeometryTypeId geometryType)
{
  vector<Tags> result;
  QVariantList l = _translateToOgrVariants(tags, elementType, geometryType);

  HandleScope handleScope;
  Context::Scope context_scope(_gContext->getContext());

  result.resize(l.size());
  for (int i = 0; i < l.size(); i++)
  {
    Tags t;
    QVariant v = l[i].toMap()["attrs"];

    if (v.isNull())
    {
      throw HootException("Expected translateToOgr result to contain 'attrs' property.");
    }

    const QVariantMap& vm = l[i].toMap()["attrs"].toMap();

    for (QVariantMap::const_iterator it = vm.constBegin(); it != vm.constEnd(); ++it)
    {
      t[it.key()] = it.value().toString();
    }

    result[i] = t;
  }

  return result;
}

QVariantList JavaScriptTranslator::_translateToOgrVariants(Tags& tags,
  ElementType elementType, geos::geom::GeometryTypeId geometryType)
{
  //LOG_DEBUG("Started translateToOgr");

  _tags = &tags;

  HandleScope handleScope;
  Context::Scope context_scope(_gContext->getContext());

  Handle<Object> v8tags = Object::New();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    v8tags->Set(toV8(it.key()), toV8(it.value()));
  }

  // Hardcoded to 3 arguments
  //  args << tagsJs << elementTypeJs << geometryTypeJs;
  Handle<Value> args[3];
  args[0] = v8tags;
  args[1] = toV8(elementType.toString());

  switch (geometryType)
  {
  case GEOS_POINT:
  case GEOS_MULTIPOINT:
    args[2] = String::New("Point");
    break;
  case GEOS_LINESTRING:
  case GEOS_MULTILINESTRING:
    args[2] = String::New("Line");
    break;
  case GEOS_POLYGON:
  case GEOS_MULTIPOLYGON:
    args[2] = String::New("Area");
    break;
  case GEOS_GEOMETRYCOLLECTION:
    args[2] = String::New("Collection");
    break;
  default:
    throw InternalErrorException("Unexpected geometry type.");
  }

  double start = 0.00; // to stop warnings
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    start = Tgs::Time::getTime();
  }

//  QScriptValue translated = _translateToOgrFunction->call(QScriptValue(), args);
  Handle<Object> tObj = _gContext->getContext()->Global();

  // We assume this exists. we checked during Init.
  Handle<v8::Function> tFunc = Handle<v8::Function>::Cast(tObj->Get(String::NewSymbol("translateToOgr")));

  // Make sure we have a translation function. No easy way to do this earlier than now
  if (tFunc->IsUndefined())
  {
    throw HootException("This translation file does not support converting to OGR. "
                        "(Missing translateToOgr)");
  }

  TryCatch trycatch;
  // Hardcoded to 3 arguments
  Handle<Value> translated = tFunc->Call(tObj, 3, args);
  HootExceptionJs::checkV8Exception(translated, trycatch);

  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    _timing.push_back((Tgs::Time::getTime() - start) * 1000.0);
  }

  QVariantList result;
  if (translated->IsNull() || translated->IsUndefined())
  {
    // leave the tags empty
  }
  else if (translated->IsObject() || translated->IsArray())
  {
    QVariant v = toCpp<QVariant>(translated);

    if (v.type() == QVariant::Map)
    {
      result = QVariantList() << v;
    }
    else if (v.type() == QVariant::List)
    {
      result = v.toList();
    }
  }
  else
  {
    throw Exception("osm2ogr: Expected either a null or an object as the result.");
  }

  return result;
}

void JavaScriptTranslator::_translateToOsm(Tags& t, const char *layerName, const char* geomType)
{
  _tags = &t;

  HandleScope handleScope;
  Context::Scope context_scope(_gContext->getContext());

  Handle<Object> tags = Object::New();
  for (Tags::const_iterator it = t.begin(); it != t.end(); ++it)
  {
    tags->Set(toV8(it.key()), toV8(it.value()));
  }

  Handle<Value> args[3];
  args[0] = tags;
  args[1] = toV8(layerName);
  args[2] = toV8(geomType);

  Handle<Object> tObj = _gContext->getContext()->Global();

  // This has a variable since we don't know if it will be "translateToOsm" or "translateAttributes"
  Handle<v8::Function> tFunc = Handle<v8::Function>::Cast(tObj->Get(toV8(_toOsmFunctionName)));
  TryCatch trycatch;

  // NOTE: the "3" here is the number of arguments
  Handle<Value> newTags = tFunc->Call(tObj, 3, args);
  HootExceptionJs::checkV8Exception(newTags, trycatch);

  double start = 0.00; // to stop warnings
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    start = Tgs::Time::getTime();
  }

  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    _timing.push_back((Tgs::Time::getTime() - start) * 1000.0);
  }

  t.clear();

  // Just making sure...
  if (newTags.IsEmpty() || newTags->IsNull() || newTags->IsUndefined())
  {
    // leave the tags empty
  }
  else if (newTags->IsObject())
  {
    // Either do this or a cast. Not sure what is better/faster
    // Handle<Object> obj = Handle<Object>::Cast(newTags);
    Handle<Object> obj = newTags->ToObject();

    Local<Array> arr = obj->GetPropertyNames();

    for (uint32_t i = 0; i < arr->Length(); i++)
    {
      Local<Value> v(obj->Get(arr->Get(i)));

      // Need to make sure the "Value" is not undefined or else "Bad Things Happen"
      if (v != Undefined())
      {
        t.insert(toCpp<QString>(arr->Get(i)), toCpp<QString>(v));
      }
    }
  }
  else
  {
    throw Exception("ogr2osm: Expected either a null or an object as the result.");
  }
}



}

