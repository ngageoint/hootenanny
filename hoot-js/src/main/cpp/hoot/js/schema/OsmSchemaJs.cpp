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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
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
  schema->Set(String::NewSymbol("getCategories"), FunctionTemplate::New(getCategories)->GetFunction());
  schema->Set(String::NewSymbol("isArea"), FunctionTemplate::New(isArea)->GetFunction());
  schema->Set(String::NewSymbol("isBuilding"), FunctionTemplate::New(isBuilding)->GetFunction());
  schema->Set(String::NewSymbol("isLinear"), FunctionTemplate::New(isLinear)->GetFunction());
  schema->Set(String::NewSymbol("isLinearWaterway"), FunctionTemplate::New(isLinearWaterway)->GetFunction());
  schema->Set(String::NewSymbol("isPoi"), FunctionTemplate::New(isPoi)->GetFunction());
  schema->Set(String::NewSymbol("score"), FunctionTemplate::New(score)->GetFunction());
}

Handle<Value> OsmSchemaJs::getCategories(const Arguments& args) {
  HandleScope scope;

  QString kvp = toCpp<QString>(args[0]);

  return scope.Close(toV8(OsmSchema::getInstance().getCategories(kvp).toStringList()));
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

Handle<Value> OsmSchemaJs::isLinearWaterway(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isLinearWaterway(*e)));
}

Handle<Value> OsmSchemaJs::isPoi(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(OsmSchema::getInstance().isPoi(*e)));
}

Handle<Value> OsmSchemaJs::score(const Arguments& args) {
  HandleScope scope;

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  return scope.Close(Number::New(OsmSchema::getInstance().score(kvp1, kvp2)));
}

}
