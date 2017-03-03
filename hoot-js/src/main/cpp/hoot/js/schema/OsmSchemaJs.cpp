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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmSchemaJs.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/DataConvertJs.h>

namespace hoot
{

HOOT_JS_REGISTER(OsmSchemaJs)

OsmSchemaJs::OsmSchemaJs()
{
}

OsmSchemaJs::~OsmSchemaJs() {}

void OsmSchemaJs::Init(Handle<Object> exports)
{
  Handle<Object> schema = Object::New();
  exports->Set(String::NewSymbol("OsmSchema"), schema);
  schema->Set(String::NewSymbol("getAllTags"), FunctionTemplate::New(getAllTags)->GetFunction());
  schema->Set(String::NewSymbol("getCategories"), FunctionTemplate::New(getCategories)->GetFunction());
  schema->Set(String::NewSymbol("getChildTags"), FunctionTemplate::New(getChildTags)->GetFunction());
  schema->Set(String::NewSymbol("getSimilarTags"),
    FunctionTemplate::New(getSimilarTags)->GetFunction());
  schema->Set(String::NewSymbol("getTagByCategory"),
    FunctionTemplate::New(getTagByCategory)->GetFunction());
  schema->Set(String::NewSymbol("getTagVertex"),
    FunctionTemplate::New(getTagVertex)->GetFunction());
  schema->Set(String::NewSymbol("isAncestor"), FunctionTemplate::New(isAncestor)->GetFunction());
  schema->Set(String::NewSymbol("isArea"), FunctionTemplate::New(isArea)->GetFunction());
  schema->Set(String::NewSymbol("isBuilding"), FunctionTemplate::New(isBuilding)->GetFunction());
  schema->Set(String::NewSymbol("isHgisPoi"), FunctionTemplate::New(isHgisPoi)->GetFunction());
  schema->Set(String::NewSymbol("isLinear"), FunctionTemplate::New(isLinear)->GetFunction());
  schema->Set(String::NewSymbol("isLinearWaterway"), FunctionTemplate::New(isLinearWaterway)->GetFunction());
  schema->Set(String::NewSymbol("isMetaData"), FunctionTemplate::New(isMetaData)->GetFunction());
  schema->Set(String::NewSymbol("isPoi"), FunctionTemplate::New(isPoi)->GetFunction());
  schema->Set(String::NewSymbol("isLinearHighway"), FunctionTemplate::New(isLinearHighway)->GetFunction());
  schema->Set(String::NewSymbol("score"), FunctionTemplate::New(score)->GetFunction());
  schema->Set(String::NewSymbol("scoreOneWay"), FunctionTemplate::New(scoreOneWay)->GetFunction());
}

Handle<Value> OsmSchemaJs::getAllTags(const Arguments& /*args*/) {
  HandleScope scope;

  Handle<Value> result = toV8(OsmSchema::getInstance().getAllTags());
  return scope.Close(result);
}

Handle<Value> OsmSchemaJs::getCategories(const Arguments& args) {
  HandleScope scope;

  QString kvp = toCpp<QString>(args[0]);

  return scope.Close(toV8(OsmSchema::getInstance().getCategories(kvp).toStringList()));
}

Handle<Value> OsmSchemaJs::getChildTags(const Arguments& args) {
  HandleScope scope;

  QString kvp = toCpp<QString>(args[0]);

  return scope.Close(toV8(OsmSchema::getInstance().getChildTags(kvp)));
}

Handle<Value> OsmSchemaJs::getSimilarTags(const Arguments& args) {
  HandleScope scope;

  QString kvp = toCpp<QString>(args[0]);
  double minimumScore = toCpp<double>(args[1]);

  return scope.Close(toV8(OsmSchema::getInstance().getSimilarTags(kvp, minimumScore)));
}

Handle<Value> OsmSchemaJs::getTagByCategory(const Arguments& args) {
  HandleScope scope;

  QString category = toCpp<QString>(args[0]);
  OsmSchemaCategory c = OsmSchemaCategory::fromString(category);

  return scope.Close(toV8(OsmSchema::getInstance().getTagByCategory(c)));
}

Handle<Value> OsmSchemaJs::getTagVertex(const Arguments& args) {
  HandleScope scope;

  QString kvp = toCpp<QString>(args[0]);

  return scope.Close(toV8(OsmSchema::getInstance().getTagVertex(kvp)));
}

Handle<Value> OsmSchemaJs::isAncestor(const Arguments& args) {
  HandleScope scope;

  QString childKvp = toCpp<QString>(args[0]);
  QString parentKvp = toCpp<QString>(args[1]);

  return scope.Close(Boolean::New(OsmSchema::getInstance().isAncestor(childKvp, parentKvp)));
}

Handle<Value> OsmSchemaJs::isArea(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isArea(e)));
}

Handle<Value> OsmSchemaJs::isLinear(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isLinear(*e)));
}

Handle<Value> OsmSchemaJs::isBuilding(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isBuilding(e)));
}

Handle<Value> OsmSchemaJs::isHgisPoi(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isHgisPoi(*e)));
}

Handle<Value> OsmSchemaJs::isLinearWaterway(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isLinearWaterway(*e)));
}

Handle<Value> OsmSchemaJs::isMetaData(const Arguments& args) {
  HandleScope scope;

  QString key = toCpp<QString>(args[0]);
  QString value = toCpp<QString>(args[1]);

  return scope.Close(Boolean::New(OsmSchema::getInstance().isMetaData(key, value)));
}

Handle<Value> OsmSchemaJs::isPoi(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isPoi(*e)));
}

Handle<Value> OsmSchemaJs::isLinearHighway(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isLinearHighway(e->getTags(), e->getElementType())));
}

Handle<Value> OsmSchemaJs::score(const Arguments& args) {
  HandleScope scope;

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  return scope.Close(Number::New(OsmSchema::getInstance().score(kvp1, kvp2)));
}

Handle<Value> OsmSchemaJs::scoreOneWay(const Arguments& args) {
  HandleScope scope;

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  return scope.Close(Number::New(OsmSchema::getInstance().scoreOneWay(kvp1, kvp2)));
}

}
