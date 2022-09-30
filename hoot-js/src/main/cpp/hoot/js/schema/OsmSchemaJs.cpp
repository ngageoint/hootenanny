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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmSchemaJs.h"

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>
#include <hoot/core/criterion/HasNameCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/RiverCriterion.h>
#include <hoot/core/schema/OsmSchema.h>

#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/elements/OsmMapJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmSchemaJs)

void OsmSchemaJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> schema = Object::New(current);
  exports->Set(context, toV8("OsmSchema"), schema);

  schema->Set(context, toV8("getAllTags"),
              FunctionTemplate::New(current, getAllTags)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("getCategories"),
              FunctionTemplate::New(current, getCategories)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("getChildTagsAsVertices"),
              FunctionTemplate::New(current, getChildTagsAsVertices)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("getSimilarTagsAsVertices"),
              FunctionTemplate::New(current, getSimilarTagsAsVertices)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("getTagVertex"),
              FunctionTemplate::New(current, getTagVertex)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isAncestor"),
              FunctionTemplate::New(current, isAncestor)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isGeneric"),
              FunctionTemplate::New(current, isGeneric)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("hasType"),
              FunctionTemplate::New(current, hasType)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("explicitTypeMismatch"),
              FunctionTemplate::New(current, explicitTypeMismatch)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("mostSpecificType"),
              FunctionTemplate::New(current, mostSpecificType)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("score"),
              FunctionTemplate::New(current, score)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("scoreTypes"),
              FunctionTemplate::New(current, scoreTypes)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("scoreOneWay"),
              FunctionTemplate::New(current, scoreOneWay)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("hasName"),
              FunctionTemplate::New(current, hasName)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isSpecificallyConflatable"),
              FunctionTemplate::New(current, isSpecificallyConflatable)->GetFunction(context).ToLocalChecked());

  schema->Set(context, toV8("isPolygon"),
              FunctionTemplate::New(current, isPolygon)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isPoint"),
              FunctionTemplate::New(current, isPoint)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isLinear"),
              FunctionTemplate::New(current, isLinear)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isRiver"),
              FunctionTemplate::New(current, isRiver)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isPowerLine"),
              FunctionTemplate::New(current, isPowerLine)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isPoi"),
              FunctionTemplate::New(current, isPoi)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isRailway"),
              FunctionTemplate::New(current, isRailway)->GetFunction(context).ToLocalChecked());
  schema->Set(context, toV8("isNonBuildingArea"),
              FunctionTemplate::New(current, isNonBuildingArea)->GetFunction(context).ToLocalChecked());
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

void OsmSchemaJs::isGeneric(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  const bool isGeneric = OsmSchema::getInstance().isGeneric(e->getTags());
  LOG_VART(isGeneric);

  args.GetReturnValue().Set(Boolean::New(current, isGeneric));
}

void OsmSchemaJs::hasType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  const bool hasType = OsmSchema::getInstance().hasType(e->getTags());
  LOG_VART(hasType);

  args.GetReturnValue().Set(Boolean::New(current, hasType));
}

void OsmSchemaJs::explicitTypeMismatch(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e1 =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();
  ConstElementPtr e2 =
    ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked())->getConstElement();
  double minimumScore = toCpp<double>(args[2]);

  const bool hasExplicitTypeMismatch =
    OsmSchema::getInstance().explicitTypeMismatch(e1->getTags(), e2->getTags(), minimumScore);

  args.GetReturnValue().Set(Boolean::New(current, hasExplicitTypeMismatch));
}

void OsmSchemaJs::isPoint(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, PointCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isLinear(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, LinearCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isPolygon(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, PolygonCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isRiver(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, RiverCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isPowerLine(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, PowerLineCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isPoi(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, PoiCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isRailway(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, RailwayCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isNonBuildingArea(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, NonBuildingAreaCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isSpecificallyConflatable(
  const FunctionCallbackInfo<Value>& args)
{
  bool isSpecificallyConflatable = false;

  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  NonConflatableCriterion crit(mapJs->getConstMap());
  crit.setIgnoreGenericConflators(true);

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked())->getConstElement();

  const QString geometryTypeFilterStr = toCpp<QString>(args[2]).trimmed();
  if (!geometryTypeFilterStr.isEmpty())
  {
    crit.setGeometryTypeFilter(GeometryTypeCriterion::typeFromString(geometryTypeFilterStr));
  }
  isSpecificallyConflatable = !crit.isSatisfied(e);
  LOG_VART(e);
  LOG_VART(isSpecificallyConflatable);

  args.GetReturnValue().Set(Boolean::New(current, isSpecificallyConflatable));
}

void OsmSchemaJs::hasName(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr e =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, HasNameCriterion().isSatisfied(e)));
}

void OsmSchemaJs::score(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(Number::New(current, OsmSchema::getInstance().score(kvp1, kvp2)));
}

void OsmSchemaJs::scoreTypes(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  Tags tags1 = toCpp<Tags>(args[0]);
  Tags tags2 = toCpp<Tags>(args[1]);
  const bool ignoreGenericTypes = toCpp<bool>(args[2]);

  const double typeScore = OsmSchema::getInstance().scoreTypes(tags1, tags2, ignoreGenericTypes);
  LOG_VART(typeScore);

  args.GetReturnValue().Set(Number::New(current, typeScore));
}

void OsmSchemaJs::scoreOneWay(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  QString kvp1 = toCpp<QString>(args[0]);
  QString kvp2 = toCpp<QString>(args[1]);

  args.GetReturnValue().Set(Number::New(current, OsmSchema::getInstance().scoreOneWay(kvp1, kvp2)));
}

void OsmSchemaJs::mostSpecificType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ConstElementPtr element =
    ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject(context).ToLocalChecked())->getConstElement();
  const QString kvp = OsmSchema::getInstance().mostSpecificType(element->getTags());
  args.GetReturnValue().Set(String::NewFromUtf8(current, kvp.toUtf8().data()).ToLocalChecked());
}

}
