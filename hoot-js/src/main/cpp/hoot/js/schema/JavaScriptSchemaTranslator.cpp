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
 * @copyright Copyright (C) 2015-2025 Maxar (http://www.maxar.com/)
 */

#include "JavaScriptSchemaTranslator.h"

// hoot
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/schema/DoubleFieldDefinition.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/IntegerFieldDefinition.h>
#include <hoot/core/io/schema/Layer.h>
#include <hoot/core/io/schema/LongIntegerFieldDefinition.h>
#include <hoot/core/io/schema/Schema.h>
#include <hoot/core/io/schema/StringFieldDefinition.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/UuidHelper.h>

#include <hoot/js/PluginContext.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QLibraryInfo>

// TBS
#include <tbs/stats/SampleStats.h>

// Tgs
#include <tgs/System/Time.h>

Q_DECLARE_METATYPE(hoot::JavaScriptSchemaTranslator*)

using namespace geos::geom;
using namespace std;
using namespace v8;

namespace hoot
{

int JavaScriptSchemaTranslator::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ScriptSchemaTranslator, JavaScriptSchemaTranslator)

JavaScriptSchemaTranslator::JavaScriptSchemaTranslator()
{
  setConfiguration(conf());
}

void JavaScriptSchemaTranslator::setConfiguration(const Settings& conf)
{
  _conf = conf;
}

JavaScriptSchemaTranslator::~JavaScriptSchemaTranslator()
{
  if (!_timing.empty())
  {
    tbs::SampleStats stats(_timing);
    LOG_TRACE("Translation script run time (ms): " << stats.toString());
  }
  close();
}

vector<JavaScriptSchemaTranslator::TranslatedFeature> JavaScriptSchemaTranslator::_createAllFeatures(const QVariantList& list) const
{
  vector<TranslatedFeature> result;
  result.reserve(list.size());

  for (const auto& feature : qAsConst(list))
  {
    TranslatedFeature tf;
    tf.feature = _createFeature(feature.toMap(), tf.tableName);
    if (tf.feature)
      result.push_back(tf);
  }

  return result;
}

std::shared_ptr<Feature> JavaScriptSchemaTranslator::_createFeature(const QVariantMap& vm, QString &tableName) const
{
  if (vm.contains("attrs") == false)
    throw HootException("Expected translateToOgr result to contain 'attrs' property.");

  if (vm.contains("tableName") == false)
    throw HootException("Expected translateToOgr result to contain 'tableName' property.");

  tableName = vm["tableName"].toString();
  if (tableName.length() < 1)
  {
    LOG_TRACE("_createFeature: Empty tableName");
    return std::shared_ptr<Feature>(); // Null feature
  }

  // figure out which layer this feature belongs in.
  std::shared_ptr<const Layer> layer;
  for (size_t i = 0; i < _schema->getLayerCount(); ++i)
  {
    std::shared_ptr<const Layer> l = _schema->getLayer(i);

    if (l->getName() == tableName)
      layer = l;
  }

  if (layer == nullptr)
    strictError("Table name: " + tableName + " not found in schema.");

  // copy the attributes into the feature.
  if (vm["attrs"].canConvert(QVariant::Map) == false)
    throw HootException("Expected attrs to be an object.");

  QVariantMap attrs = vm["attrs"].toMap();

  std::shared_ptr<Feature> result = std::make_shared<Feature>(layer->getFeatureDefinition());

  for (auto it = attrs.begin(); it != attrs.end(); ++it)
  {
    if (result->getFeatureDefinition()->hasField(it.key()) == false)
    {
      QString error("The specified field/column is not part of this table definition."
                    " table: " + tableName + " column: " + it.key());
      if (_strict == StrictOn)
        throw FieldDefinition::InvalidValueException(it.key(), error);
      else
      {
        LOG_WARN(error);
      }
    }
    else
      result->setField(it.key(), it.value());
  }
  result->validate(_strict);
  return result;
}

void JavaScriptSchemaTranslator::_finalize()
{
  // if we didn't error out.
  if (!_error)
  {
    // run the user's finalize function if it exists.
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_gContext->getContext(current));
    Local<Context> context = current->GetCurrentContext();

    Local<Object> tObj = _gContext->getContext(current)->Global();

    if (tObj->Has(context, toV8("finalize")).ToChecked())
    {
      TryCatch trycatch(current);
      Local<Value> finalize = _gContext->call(tObj, "finalize");
      HootExceptionJs::checkV8Exception(finalize, trycatch);
    }
  }

  _initialized = false;
}

void JavaScriptSchemaTranslator::_init()
{
  // This can be a costly operation, hence putting it at INFO for runtime awareness purposes.
  LOG_INFO("Loading translation script: " << _scriptPath << "...");

  _error = false;
  _gContext = std::make_shared<PluginContext>();
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_gContext->getContext(current));
  Local<Context> context = current->GetCurrentContext();

  if (_scriptPath.isEmpty())
  {
    if (_scriptText.isEmpty())
      throw HootException("Please either specify a script path or text.");
    else
      _gContext->loadText(_scriptText);
  }
  else
    _gContext->loadScript(_scriptPath);

  // Less typing
  Local<Object> tObj = _gContext->getContext(current)->Global();

  // Run Initialize, if it exists
  if (tObj->Has(context, toV8("initialize")).ToChecked())
  {
    TryCatch trycatch(current);
    Local<Value> initial = _gContext->call(tObj,"initialize");
    HootExceptionJs::checkV8Exception(initial, trycatch);
  }

  // Sort out what the toOsm function is called
  if (tObj->Has(context, toV8("translateToOsm")).ToChecked())
    _toOsmFunctionName = "translateToOsm";
  else if (tObj->Has(context, toV8("translateAttributes")).ToChecked())
    _toOsmFunctionName = "translateAttributes";

  LOG_DEBUG("Translation script loaded.");
  _initialized = true;
}

// Use the layerNameFilter function to get the filter string (regexp)
QString JavaScriptSchemaTranslator::getLayerNameFilter()
{
  // Just making sure
  if (!_initialized)
    _init();

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_gContext->getContext(current));
  Local<Context> context = current->GetCurrentContext();

  Local<Object> tObj = _gContext->getContext(current)->Global();

  LOG_TRACE("Getting layer name filter...");
  if (tObj->Has(context, toV8("layerNameFilter")).ToChecked())
    return toCpp<QString>(_gContext->call(tObj,"layerNameFilter"));
  else
    return ScriptSchemaTranslator::getLayerNameFilter();
}

bool JavaScriptSchemaTranslator::isValidScript()
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
      result = true;
  }
  return result;
}

std::shared_ptr<const Schema> JavaScriptSchemaTranslator::getOgrOutputSchema()
{
  LOG_TRACE("Started getOgrOutputSchema");

  if (_schema == nullptr)
  {
    if (!_initialized)
      _init();

    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_gContext->getContext(current));
    Local<Context> context = current->GetCurrentContext();

    Local<Object> tObj = _gContext->getContext(current)->Global();

    if (!tObj->Has(context, toV8("getDbSchema")).ToChecked())
      throw HootException("This translation file does not support converting to OGR. (Missing schema): ..." + _scriptPath.right(25));

    Local<Value> schemaJs(_gContext->call(tObj, "getDbSchema"));

    if (schemaJs->IsArray())
    {
      std::shared_ptr<Schema> schema = std::make_shared<Schema>();
      QVariantList schemaV = toCpp<QVariant>(schemaJs).toList();

      for (const auto& variant : qAsConst(schemaV))
        schema->addLayer(_parseLayer(variant));

      _schema = schema;
    }
    else
      throw HootException("Expected a valid schema array to be returned.");
  }
  return _schema;
}

void JavaScriptSchemaTranslator::_parseEnumerations(std::shared_ptr<DoubleFieldDefinition> fd, const QVariant& enumerations) const
{
  if (enumerations.canConvert(QVariant::List) == false)
    throw HootException("Expected enumerations to be an array of maps.");

  QVariantList vl = enumerations.toList();

  for (const auto& variant : qAsConst(vl))
  {
    if (variant.canConvert(QVariant::Map) == false)
      throw HootException("Expected enumerations to be an array of maps.");

    QVariantMap vm = variant.toMap();
    if (vm["value"].canConvert(QVariant::Double) == false)
      throw HootException("Expected each enumeration map to contain a valid value.");

    double v = vm["value"].toDouble();

    if (fd->hasEnumeratedValue(v))
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Enumerated value repeated in enumerations table: " << v);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
      fd->addEnumeratedValue(v);
  }
}

void JavaScriptSchemaTranslator::_parseEnumerations(std::shared_ptr<StringFieldDefinition> fd, const QVariant& enumerations) const
{
  if (enumerations.canConvert(QVariant::List) == false)
    throw HootException("Expected enumerations to be an array of maps.");

  QVariantList vl = enumerations.toList();

  for (const auto& variant : qAsConst(vl))
  {
    if (variant.canConvert(QVariant::Map) == false)
      throw HootException("Expected enumerations to be an array of maps.");

    QVariantMap vm = variant.toMap();
    if (vm["name"].canConvert(QVariant::String) == false)
      throw HootException("Expected each enumeration map to contain a valid value.");

    QString v = vm["name"].toString();

    if (fd->hasEnumeratedValue(v))
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Enumerated value repeated in enumerations table: " << v);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
      fd->addEnumeratedValue(v);
  }
}

void JavaScriptSchemaTranslator::_parseEnumerations(std::shared_ptr<IntegerFieldDefinition> fd,
                                                    const QVariant& enumerations) const
{
  if (enumerations.canConvert(QVariant::List) == false)
    throw HootException("Expected enumerations to be an array of maps.");

  QVariantList vl = enumerations.toList();
  for (const auto& variant : qAsConst(vl))
  {
    if (variant.canConvert(QVariant::Map) == false)
      throw HootException("Expected enumerations to be an array of maps.");

    QVariantMap vm = variant.toMap();
    if (vm["value"].canConvert(QVariant::Int) == false)
      throw HootException("Expected each enumeration map to contain a valid value.");

    int v = vm["value"].toInt();
    if (fd->hasEnumeratedValue(v))
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Enumerated value repeated in enumerations table: " << v);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
      fd->addEnumeratedValue(v);
  }
}

void JavaScriptSchemaTranslator::_parseEnumerations(std::shared_ptr<LongIntegerFieldDefinition> fd,
                                                    const QVariant& enumerations) const
{
  if (enumerations.canConvert(QVariant::List) == false)
    throw HootException("Expected enumerations to be an array of maps.");

  QVariantList vl = enumerations.toList();

  for (const auto& variant : qAsConst(vl))
  {
    if (variant.canConvert(QVariant::Map) == false)
      throw HootException("Expected enumerations to be an array of maps.");

    QVariantMap vm = variant.toMap();
    if (vm["value"].canConvert(QVariant::LongLong) == false)
      throw HootException("Expected each enumeration map to contain a valid value.");

    qlonglong v = vm["value"].toLongLong();
    if (fd->hasEnumeratedValue(v))
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Enumerated value repeated in enumerations table: " << v);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    else
      fd->addEnumeratedValue(v);
  }
}

std::shared_ptr<FieldDefinition> JavaScriptSchemaTranslator::_parseFieldDefinition(const QVariant& fieldV) const
{
  std::shared_ptr<FieldDefinition> result;

  if (fieldV.canConvert(QVariant::Map) == false)
    throw HootException("Expected the column to be an object (map).");

  QVariantMap map = fieldV.toMap();
  if (map.contains("type") == false)
    throw HootException("Error parsing type in column.");

  QString type = map["type"].toString().toLower();
  if (type == "string" || (type == "enumeration" && !ConfigOptions().getOgrCodedValues()))
  {
    std::shared_ptr<StringFieldDefinition> fd = std::make_shared<StringFieldDefinition>();

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
        fd->setDefaultIsNull(true);
      else
        fd->setDefaultValue(map["defValue"].toString());
    }
    if (map.contains(MetadataTags::Length()))
      fd->setWidth(_toInt32(map[MetadataTags::Length()]));

    if (map.contains("enumerations"))
    {
      _parseEnumerations(fd, map["enumerations"]);
    }
    result = fd;
  }
  else if (type == "double" || type == "real")
  {
    std::shared_ptr<DoubleFieldDefinition> fd = std::make_shared<DoubleFieldDefinition>();

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
        fd->setDefaultIsNull(true);
      else
        fd->setDefaultValue(_toDouble(map["defValue"]));
    }
    if (map.contains("maximum"))
      fd->setMaxValue(_toDouble(map["maximum"]));

    if (map.contains("minimum"))
      fd->setMinValue(_toDouble(map["minimum"]));

    if (map.contains("enumerations"))
      _parseEnumerations(fd, map["enumerations"]);

    result = fd;
  }
  else if (type == "enumeration" || type == "integer")
  {
    std::shared_ptr<IntegerFieldDefinition> fd = std::make_shared<IntegerFieldDefinition>();

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
        fd->setDefaultIsNull(true);
      else
        fd->setDefaultValue(_toInt32(map["defValue"]));
    }

    if (map.contains("maximum"))
      fd->setMaxValue(_toInt32(map["maximum"]));

    if (map.contains("minimum"))
      fd->setMinValue(_toInt32(map["minimum"]));

    if (map.contains("enumerations"))
      _parseEnumerations(fd, map["enumerations"]);

    result = fd;
  }
  else if (type == "long integer")
  {
    std::shared_ptr<LongIntegerFieldDefinition> fd = std::make_shared<LongIntegerFieldDefinition>();

    if (map.contains("defValue"))
    {
      if (map["defValue"].isValid() == false)
        fd->setDefaultIsNull(true);
      else
        fd->setDefaultValue(_toInt64(map["defValue"]));
    }

    if (map.contains("maximum"))
      fd->setMaxValue(_toInt64(map["maximum"]));

    if (map.contains("minimum"))
      fd->setMinValue(_toInt64(map["minimum"]));

    if (map.contains("enumerations"))
      _parseEnumerations(fd, map["enumerations"]);

    result = fd;
  }
  else
    throw HootException("Unexpected column type: " + type + " for " + map["name"].toString());

  if (map.contains("name") == false)
    throw HootException("Error parsing name in column.");

  result->setName(map["name"].toString());
  if (map.contains("allowNull"))
  {
    QString s = map["allowNull"].toString().toLower();
    if (s == "true")
      result->setAllowNull(true);
    else if (s == "false")
      result->setAllowNull(false);
    else
      throw HootException("Expected a value of 'true' or 'false' for allowNull.");
  }
  else if (map["name"].toString() != "OSMTAGS" && ConfigOptions().getOgrAppendData())
    result->setAllowNull(false);

  return result;
}

std::shared_ptr<Layer> JavaScriptSchemaTranslator::_parseLayer(const QVariant& layer) const
{
  std::shared_ptr<Layer> newLayer = std::make_shared<Layer>();

  if (layer.canConvert(QVariant::Map) == false)
    throw HootException("Expected the layer to be an object (map).");

  QVariantMap map = layer.toMap();

  // Parse out the name of the layer.
  if (map.contains("name") == false)
    throw HootException("Expected layer to contain a name.");

  newLayer->setName(map["name"].toString());

  // Parse out the geometry portion of the layer.
  if (map.contains("geom") == false)
    throw HootException(QString("Expected layer (%1) to contain a geom.").arg(newLayer->getName()));

  QString geom = map["geom"].toString().toLower();
  if (geom == "line" || geom == "curve")
    newLayer->setGeometryType(GEOS_LINESTRING);
  else if (geom == "point")
    newLayer->setGeometryType(GEOS_POINT);
  else if (geom == "area" || geom == "polygon" || geom == "surface")
    newLayer->setGeometryType(GEOS_POLYGON);
  else
    throw HootException(QString("Expected layer (%1) to contain a valid geom.").arg(newLayer->getName()));

  if (map.contains("fdname") == true)
    newLayer->setFdName(map["fdname"].toString());

  // parse out the columns one at a time.
  QVariant columnsV = map["columns"];
  if (columnsV.canConvert(QVariant::List) == false)
    throw HootException(QString("Expected layer (%1) to contain a list of columns.").arg(newLayer->getName()));

  set<QString> names;
  QVariantList columns = columnsV.toList();
  std::shared_ptr<FeatureDefinition> dfd = std::make_shared<FeatureDefinition>();
  for (const auto& variant : qAsConst(columns))
  {
    std::shared_ptr<FieldDefinition> fd = _parseFieldDefinition(variant);
    if (names.find(fd->getName()) != names.end())
      throw HootException("Found multiple fields with the same name. (" + fd->getName() + ")");
    dfd->addField(fd);
    names.insert(fd->getName());
  }
  newLayer->setFeatureDefinition(dfd);

  return newLayer;
}

qint32 JavaScriptSchemaTranslator::_toInt32(const QVariant& v) const
{
  if (v.canConvert(QVariant::Int) == false)
    throw HootException("Error parsing int: " + v.toString());

  return v.toInt();
}

qint64 JavaScriptSchemaTranslator::_toInt64(const QVariant& v) const
{
  if (v.canConvert(QVariant::LongLong) == false)
    throw HootException("Error parsing long int: " + v.toString());

  return v.toLongLong();
}

double JavaScriptSchemaTranslator::_toDouble(const QVariant& v) const
{
  if (v.canConvert(QVariant::Double) == false)
    throw HootException("Error parsing double: " + v.toString());

  return v.toDouble();
}

vector<ScriptToOgrSchemaTranslator::TranslatedFeature> JavaScriptSchemaTranslator::translateToOgr(Tags& tags, ElementType elementType,
                                                                                                  geos::geom::GeometryTypeId geometryType)
{
  return _createAllFeatures(_translateToOgrVariants(tags, elementType, geometryType));
}

vector<Tags> JavaScriptSchemaTranslator::translateToOgrTags(Tags& tags, ElementType elementType,
                                                            GeometryTypeId geometryType)
{
  vector<Tags> result;
  QVariantList l = _translateToOgrVariants(tags, elementType, geometryType);

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_gContext->getContext(current));

  result.resize(l.size());
  for (int i = 0; i < l.size(); i++)
  {
    Tags t;
    QVariant v = l[i].toMap()["attrs"];

    if (v.isNull())
      throw HootException("Expected translateToOgr result to contain 'attrs' property.");

    const QVariantMap& vm = l[i].toMap()["attrs"].toMap();

    for (auto it = vm.constBegin(); it != vm.constEnd(); ++it)
    {
      const QString key = it.key();
      const QString val = it.value().toString().trimmed();
      if (!val.isEmpty())
        t[key] = val;
    }

    result[i] = t;
  }

  return result;
}

QVariantList JavaScriptSchemaTranslator::_translateToOgrVariants(const Tags& tags, ElementType elementType,
                                                                 geos::geom::GeometryTypeId geometryType)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_gContext->getContext(current));
  Local<Context> context = current->GetCurrentContext();

  Local<Object> v8tags = Object::New(current);
  for (auto it = tags.begin(); it != tags.end(); ++it)
    v8tags->Set(context, toV8(it.key()), toV8(it.value()));

  // Hardcoded to 3 arguments
  //  args << tagsJs << elementTypeJs << geometryTypeJs;
  Local<Value> args[3];
  args[0] = v8tags;
  args[1] = toV8(elementType.toString());

  switch (geometryType)
  {
  case GEOS_POINT:
  case GEOS_MULTIPOINT:
    args[2] = toV8("Point");
    break;
  case GEOS_LINESTRING:
  case GEOS_MULTILINESTRING:
    args[2] = toV8("Line");
    break;
  case GEOS_POLYGON:
  case GEOS_MULTIPOLYGON:
    args[2] = toV8("Area");
    break;
  case GEOS_GEOMETRYCOLLECTION:
    args[2] = toV8("Collection");
    break;
  default:
    throw InternalErrorException("Unexpected geometry type.");
  }

  double start = 0.00; // to stop warnings
  if (Log::getInstance().getLevel() <= Log::Debug)
    start = Tgs::Time::getTime();

  Local<Object> tObj = _gContext->getContext(current)->Global();

  // We assume this exists. we checked during Init.
  Local<Function> tFunc = Local<Function>::Cast(tObj->Get(context, toV8("translateToOgr")).ToLocalChecked());

  // Make sure we have a translation function. No easy way to do this earlier than now
  if (tFunc->IsUndefined())
    throw HootException("This translation file does not support converting to OGR. (Missing translateToOgr)");

  TryCatch trycatch(current);
  // Hardcoded to 3 arguments
  MaybeLocal<Value> maybe_translated = tFunc->Call(context, tObj, 3, args);

  if (maybe_translated.IsEmpty())
    HootExceptionJs::throwAsHootException(trycatch);

  Local<Value> translated = maybe_translated.ToLocalChecked();

  HootExceptionJs::checkV8Exception(translated, trycatch);

  if (Log::getInstance().getLevel() <= Log::Debug)
    _timing.push_back((Tgs::Time::getTime() - start) * 1000.0);

  QVariantList result;
  if (translated->IsNull() || translated->IsUndefined())
  {
    // leave the tags empty
  }
  else if (translated->IsObject() || translated->IsArray())
  {
    QVariant v = toCpp<QVariant>(translated);

    if (v.type() == QVariant::Map)
      result = QVariantList() << v;
    else if (v.type() == QVariant::List)
      result = v.toList();
  }
  else
    throw HootException("convert: Expected either a null or an object as the result.");

  return result;
}

void JavaScriptSchemaTranslator::_translateToOsm(Tags& tags, const char* layerName, const char* geomType)
{
  LOG_VART(_toOsmFunctionName);
  LOG_VART(layerName);
  LOG_VART(geomType);

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(_gContext->getContext(current));
  Local<Context> context = current->GetCurrentContext();

  Local<Object> v8Tags = Object::New(current);
  for (auto it = tags.begin(); it != tags.end(); ++it)
  {
    LOG_VART(it.key());
    LOG_VART(it.value());
    v8Tags->Set(context, toV8(it.key()), toV8(it.value()));
  }

  Local<Value> args[3];
  args[0] = v8Tags;
  args[1] = toV8(layerName);
  args[2] = toV8(geomType);

  Local<Object> tObj = _gContext->getContext(current)->Global();

  // This has a variable since we don't know if it will be "translateToOsm" or "translateAttributes"
  Local<Function> tFunc = Local<Function>::Cast(tObj->Get(context, toV8(_toOsmFunctionName)).ToLocalChecked());
  TryCatch trycatch(current);

  // NOTE: the "3" here is the number of arguments
  MaybeLocal<Value> maybe_newTags = tFunc->Call(context, tObj, 3, args);
  if (maybe_newTags.IsEmpty())
    HootExceptionJs::throwAsHootException(trycatch);

  Local<Value> newTags = maybe_newTags.ToLocalChecked();
  HootExceptionJs::checkV8Exception(newTags, trycatch);

  double start = 0.00; // to stop warnings
  if (Log::getInstance().getLevel() <= Log::Debug)
    start = Tgs::Time::getTime();

  if (Log::getInstance().getLevel() <= Log::Debug)
    _timing.push_back((Tgs::Time::getTime() - start) * 1000.0);

  tags.clear();

  // Just making sure...
  LOG_VART(newTags.IsEmpty() || newTags->IsNull() || newTags->IsUndefined());
  LOG_VART(newTags->IsObject());
  if (newTags.IsEmpty() || newTags->IsNull() || newTags->IsUndefined())
  {
    // leave the tags empty
  }
  else if (newTags->IsObject())
  {
    // Either do this or a cast. Not sure what is better/faster
    // Local<Object> obj = Local<Object>::Cast(newTags);
    Local<Object> obj = newTags->ToObject(context).ToLocalChecked();

    Local<Array> arr = obj->GetPropertyNames(context).ToLocalChecked();

    for (uint32_t i = 0; i < arr->Length(); i++)
    {
      Local<Value> v(obj->Get(context, arr->Get(context, i).ToLocalChecked()).ToLocalChecked());

      // Need to make sure the "Value" is not undefined or else "Bad Things Happen"
      if (v != Undefined(current))
        tags.insert(getStringLocation(toCpp<QString>(arr->Get(context, i).ToLocalChecked())), getStringLocation(toCpp<QString>(v)));

      LOG_VART(tags);
    }
  }
  else
    throw HootException("convert: Expected either a null or an object as the result.");
}

}

