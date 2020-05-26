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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmSchemaJs.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/HasNameCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/criterion/NonBuildingAreaCriterion.h>
#include <hoot/core/criterion/CollectionRelationCriterion.h>
#include <hoot/core/conflate/river/RiverSnapMerger.h>

#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/elements/OsmMapJs.h>

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
  schema->Set(String::NewFromUtf8(current, "isGeneric"),
              FunctionTemplate::New(current, isGeneric)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "hasType"),
              FunctionTemplate::New(current, hasType)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "explicitTypeMismatch"),
              FunctionTemplate::New(current, explicitTypeMismatch)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "mostSpecificType"),
              FunctionTemplate::New(current, mostSpecificType)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isArea"),
              FunctionTemplate::New(current, isArea)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isPolygon"),
              FunctionTemplate::New(current, isPolygon)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isPoint"),
              FunctionTemplate::New(current, isPoint)->GetFunction());
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
  schema->Set(String::NewFromUtf8(current, "isHighway"),
              FunctionTemplate::New(current, isHighway)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isNonBuildingArea"),
              FunctionTemplate::New(current, isNonBuildingArea)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isCollectionRelation"),
              FunctionTemplate::New(current, isCollectionRelation)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "score"),
              FunctionTemplate::New(current, score)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "scoreTypes"),
              FunctionTemplate::New(current, scoreTypes)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "scoreOneWay"),
              FunctionTemplate::New(current, scoreOneWay)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "hasName"),
              FunctionTemplate::New(current, hasName)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isSpecificallyConflatable"),
              FunctionTemplate::New(current, isSpecificallyConflatable)->GetFunction());
  schema->Set(String::NewFromUtf8(current, "isLongRiverPair"),
              FunctionTemplate::New(current, isLongRiverPair)->GetFunction());
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

void OsmSchemaJs::isGeneric(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  const bool isGeneric = OsmSchema::getInstance().isGeneric(e->getTags());
  LOG_VART(isGeneric);

  args.GetReturnValue().Set(Boolean::New(current, isGeneric));
}

void OsmSchemaJs::hasType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  const bool hasType = OsmSchema::getInstance().hasType(e->getTags());
  LOG_VART(hasType);

  args.GetReturnValue().Set(Boolean::New(current, hasType));
}

void OsmSchemaJs::explicitTypeMismatch(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e1 = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();
  ConstElementPtr e2 = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();
  double minimumScore = toCpp<double>(args[2]);

  const bool hasExplicitTypeMismatch =
    OsmSchema::getInstance().explicitTypeMismatch(e1->getTags(), e2->getTags(), minimumScore);

  args.GetReturnValue().Set(Boolean::New(current, hasExplicitTypeMismatch));
}

void OsmSchemaJs::isArea(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, AreaCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isPoint(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, PointCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isLinear(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, LinearCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isPolygon(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, PolygonCriterion().isSatisfied(e)));
}

void OsmSchemaJs::isBuilding(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, BuildingCriterion(mapJs->getConstMap()).isSatisfied(e)));
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

void OsmSchemaJs::isHighway(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, HighwayCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isNonBuildingArea(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, NonBuildingAreaCriterion(mapJs->getConstMap()).isSatisfied(e)));
}

void OsmSchemaJs::isCollectionRelation(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, CollectionRelationCriterion().isSatisfied(e)));
}
void OsmSchemaJs::isSpecificallyConflatable(
  const FunctionCallbackInfo<Value>& args)
{
  bool isSpecificallyConflatable = false;

  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  NonConflatableCriterion crit(mapJs->getConstMap());
  crit.setIgnoreGenericConflators(true);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();

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

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

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

  ConstElementPtr element = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();
  const QString kvp = OsmSchema::getInstance().mostSpecificType(element->getTags());
  args.GetReturnValue().Set(String::NewFromUtf8(current, kvp.toUtf8().data()));
}

void OsmSchemaJs::isLongRiverPair(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ConstElementPtr e1 = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();
  ConstElementPtr e2 = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject())->getConstElement();

  args.GetReturnValue().Set(
    Boolean::New(current, RiverSnapMerger().isLongPair(mapJs->getConstMap(), e1, e2)));
}

}
