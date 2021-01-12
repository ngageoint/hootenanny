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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RelationMemberUtilsJs.h"

// hoot
#include <hoot/core/conflate/merging/RelationMerger.h>
#include <hoot/core/elements/RelationMemberNodeCounter.h>
#include <hoot/core/elements/ConnectedRelationMemberFinder.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/elements/ElementIdJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(RelationMemberUtilsJs)

void RelationMemberUtilsJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> obj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "RelationMemberUtils"), obj);

  obj->Set(String::NewFromUtf8(current, "isMemberOfRelationType"),
           FunctionTemplate::New(current, isMemberOfRelationType)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "isMemberOfRelationInCategory"),
           FunctionTemplate::New(current, isMemberOfRelationInCategory)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "isMemberOfRelationWithTagKey"),
           FunctionTemplate::New(current, isMemberOfRelationWithTagKey)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "getNumRelationMemberNodes"),
           FunctionTemplate::New(current, getNumRelationMemberNodes)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "relationsHaveConnectedWayMembers"),
           FunctionTemplate::New(current, relationsHaveConnectedWayMembers)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "isMemberOfRelationSatisfyingCriterion"),
           FunctionTemplate::New(current, isMemberOfRelationSatisfyingCriterion)->GetFunction());
  obj->Set(String::NewFromUtf8(current, "relationHasMember"),
           FunctionTemplate::New(current, relationHasMember)->GetFunction());
}

void RelationMemberUtilsJs::isMemberOfRelationType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementIdJs* childIdJs = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  QString relationType = toCpp<QString>(args[2]);

  const bool inRelationOfSpecifiedType =
    RelationMemberUtils::isMemberOfRelationType(map, childIdJs->getElementId(), relationType);

  args.GetReturnValue().Set(Boolean::New(current, inRelationOfSpecifiedType));
}

void RelationMemberUtilsJs::isMemberOfRelationInCategory(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementIdJs* childIdJs = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  QString schemaCategory = toCpp<QString>(args[2]);

  const bool inRelationOfSpecifiedCategory =
    RelationMemberUtils::isMemberOfRelationInCategory(
      map, childIdJs->getElementId(), schemaCategory);
  LOG_VART(inRelationOfSpecifiedCategory);

  args.GetReturnValue().Set(Boolean::New(current, inRelationOfSpecifiedCategory));
}

void RelationMemberUtilsJs::isMemberOfRelationWithTagKey(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementIdJs* childIdJs = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  QString tagKey = toCpp<QString>(args[2]);

  const bool inRelationWithSpecifiedTagKey =
    RelationMemberUtils::isMemberOfRelationWithTagKey(map, childIdJs->getElementId(), tagKey);

  args.GetReturnValue().Set(Boolean::New(current, inRelationWithSpecifiedTagKey));
}

void RelationMemberUtilsJs::getNumRelationMemberNodes(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementIdJs* relationIdJs = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  ConstRelationPtr relation = map->getRelation(relationIdJs->getElementId().getId());
  int numNodes = 0;
  if (relation)
  {
    RelationMemberNodeCounter counter;
    counter.setOsmMap(map.get());
    numNodes = counter.numNodes(relation);
  }
  args.GetReturnValue().Set(Number::New(current, numNodes));
}

void RelationMemberUtilsJs::relationsHaveConnectedWayMembers(
  const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementIdJs* relationId1Js = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  ElementIdJs* relationId2Js = ObjectWrap::Unwrap<ElementIdJs>(args[2]->ToObject());
  if (relationId1Js->getElementId().getType() != ElementType::Relation ||
      relationId2Js->getElementId().getType() != ElementType::Relation)
  {
    throw IllegalArgumentException(
      "Passed non-relation ID to relationsHaveConnectedWayMembers.");
  }

  ConnectedRelationMemberFinder finder;
  finder.setOsmMap(map.get());
  args.GetReturnValue().Set(
    Boolean::New(
      current,
        finder.haveConnectedWayMembers(
          map->getRelation(relationId1Js->getElementId().getId()),
          map->getRelation(relationId2Js->getElementId().getId()))));
}

void RelationMemberUtilsJs::isMemberOfRelationSatisfyingCriterion(
  const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapPtr map = toCpp<OsmMapPtr>(args[0]);
  ElementIdJs* childIdJs = ObjectWrap::Unwrap<ElementIdJs>(args[1]->ToObject());
  LOG_VART(childIdJs->getElementId());
  QString critClassName = toCpp<QString>(args[2]);
  LOG_VART(critClassName);

  ElementCriterionPtr crit =
    std::shared_ptr<ElementCriterion>(
      Factory::getInstance().constructObject<ElementCriterion>(critClassName.trimmed()));
  if (!crit)
  {
    throw IllegalArgumentException(
      "isMemberOfRelationSatisfyingCriterion: invalid criterion: " + critClassName.trimmed());
  }
  std::shared_ptr<OsmMapConsumer> mapConsumer =
    std::dynamic_pointer_cast<OsmMapConsumer>(crit);
  LOG_VART(mapConsumer.get());
  if (mapConsumer)
  {
    mapConsumer->setOsmMap(map.get());
  }

  const bool isMember =
    RelationMemberUtils::isMemberOfRelationSatisfyingCriterion(
      map, childIdJs->getElementId(), *crit);

  args.GetReturnValue().Set(Boolean::New(current, isMember));
}

void RelationMemberUtilsJs::relationHasMember(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    Context::Scope context_scope(current->GetCurrentContext());

    ConstElementPtr element = toCpp<ConstElementPtr>(args[0]);
    if (element->getElementType() != ElementType::Relation)
    {
      args.GetReturnValue().Set(
        current->ThrowException(
          Exception::TypeError(String::NewFromUtf8(current, "Expected a relation"))));
    }
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);

    const QString boundsStr = toCpp<QString>(args[1]);
    std::shared_ptr<geos::geom::Geometry> bounds;
    if (!boundsStr.trimmed().isEmpty())
    {
      bounds = GeometryUtils::boundsFromString(boundsStr);
    }

    const QString geometricRelationshipStr = toCpp<QString>(args[2]);
    GeometricRelationship geometricRelationship = GeometricRelationship::Invalid;
    if (!geometricRelationshipStr.trimmed().isEmpty())
    {
      geometricRelationship = GeometricRelationship::fromString(geometricRelationshipStr);
    }

    const bool filterBasedOnActiveMatchers = toCpp<bool>(args[3]);

    ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[4]);
    if (!MapProjector::isGeographic(map))
    {
      // The bounds is always in WGS84, so if our map isn't currently in WGS84 we need to reproject
      // the bounds.
      std::shared_ptr<OGRSpatialReference> srs84(new OGRSpatialReference());
      srs84->SetWellKnownGeogCS("WGS84");
      MapProjector::project(bounds, srs84, map->getProjection());
      LOG_VART(bounds);
    }

    args.GetReturnValue().Set(
      Boolean::New(
        current,
        RelationMemberUtils::relationHasMember(
          relation, bounds, geometricRelationship, filterBasedOnActiveMatchers, map)));
  }
  catch (const HootException& err)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

}
