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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmSchemaJs.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/HasNameCriterion.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmSchemaJs)

OsmSchemaJs::OsmSchemaJs()
{
}

OsmSchemaJs::~OsmSchemaJs() {}

void OsmSchemaJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> schema = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "OsmSchema"), schema);
  schema->Set(String::NewFromUtf8(current, "getAllTags"),
              FunctionTemplate::New(current, getAllTags)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "getCategories"),
              FunctionTemplate::New(current, getCategories)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "getChildTagsAsVertices"),
              FunctionTemplate::New(current, getChildTagsAsVertices)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "getSimilarTagsAsVertices"),
              FunctionTemplate::New(current, getSimilarTagsAsVertices)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "getTagByCategory"),
              FunctionTemplate::New(current, getTagByCategory)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "getTagVertex"),
              FunctionTemplate::New(current, getTagVertex)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isAncestor"),
              FunctionTemplate::New(current, isAncestor)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isArea"),
              FunctionTemplate::New(current, isArea)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isBuilding"),
              FunctionTemplate::New(current, isBuilding)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isLinear"),
              FunctionTemplate::New(current, isLinear)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isLinearWaterway"),
              FunctionTemplate::New(current, isLinearWaterway)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isPowerLine"),
              FunctionTemplate::New(current, isPowerLine)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isMetaData"),
              FunctionTemplate::New(current, isMetaData)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isPoi"),
              FunctionTemplate::New(current, isPoi)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isRailway"),
              FunctionTemplate::New(current, isRailway)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isLinearHighway"),
              FunctionTemplate::New(current, isLinearHighway)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "score"),
              FunctionTemplate::New(current, score)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "scoreOneWay"),
              FunctionTemplate::New(current, scoreOneWay)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "hasName"),
              FunctionTemplate::New(current, hasName)->GetFunction());
}

void OsmSchemaJs::getAllTags(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  args.GetReturnValue().Set(toV8(OsmSchema::getInstance().getAllTags()));
}

void OsmSchemaJs::getCategories(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString kvp = toCpp<QString>(args[0]);

  args.GetReturnValue().Set(toV8(OsmSchema::getInstance().getCategories(kvp).toStringList()));
}

void OsmSchemaJs::getChildTagsAsVertices(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString kvp = toCpp<QString>(args[0]);

  args.GetReturnValue().Set(toV8(OsmSchema::getInstance().getChildTagsAsVertices(kvp)));
}

void OsmSchemaJs::getSimilarTagsAsVertices(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString kvp = toCpp<QString>(args[0]);
  double minimumScore = toCpp<double>(args[1]);

  args.GetReturnValue().Set(
    toV8(OsmSchema::getInstance().getSimilarTagsAsVertices(kvp, minimumScore)));
}

void OsmSchemaJs::getTagByCategory(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString category = toCpp<QString>(args[0]);
  OsmSchemaCategory c = OsmSchemaCategory::fromString(category);

  args.GetReturnValue().Set(toV8(OsmSchema::getInstance().getTagByCategory(c)));
}

void OsmSchemaJs::getTagVertex(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString kvp = toCpp<QString>(args[0]);

  args.GetReturnValue().Set(toV8(OsmSchema::getInstance().getTagVertex(kvp)));
}

void OsmSchemaJs::isAncestor(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString childKvp = toCpp<QString>(args[0]);
  QString parentKvp = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(
    Boolean::New(current, OsmSchema::getInstance().isAncestor(childKvp, parentKvp)));
}

void OsmSchemaJs::isArea(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, AreaCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isLinear(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, LinearCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isBuilding(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, BuildingCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isLinearWaterway(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, LinearWaterwayCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isPowerLine(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, PowerLineCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isMetaData(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString key = toCpp<QString>(args[0]);
  QString value = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(Boolean::New(current, OsmSchema::getInstance().isMetaData(key, value)));
}

void OsmSchemaJs::isPoi(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, PoiCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isRailway(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, RailwayCriterion().isSatisfied(e)));
}

void OsmSchemaJs::hasName(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, HasNameCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isLinearHighway(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, HighwayCriterion().isSatisfied(e)));
}

void OsmSchemaJs::score(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(Number::New(current, OsmSchema::getInstance().score(kvp1, kvp2)));
}

void OsmSchemaJs::scoreOneWay(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(Number::New(current, OsmSchema::getInstance().scoreOneWay(kvp1, kvp2)));
}

}
