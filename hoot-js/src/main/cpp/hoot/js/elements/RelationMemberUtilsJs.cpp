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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "RelationMemberUtilsJs.h"

// hoot
#include <hoot/core/conflate/merging/RelationMerger.h>
#include <hoot/core/elements/RelationMemberNodeCounter.h>
#include <hoot/core/elements/ConnectedRelationMemberFinder.h>
#include <hoot/core/elements/RelationMemberUtils.h>
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

void RelationMemberUtilsJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> obj = Object::New(current);
  exports->Set(context, toV8("RelationMemberUtils"), obj);

  obj->Set(context, toV8("isMemberOfRelationWithType"),
           FunctionTemplate::New(current, isMemberOfRelationWithType)->GetFunction(context).ToLocalChecked());
  obj->Set(context, toV8("isMemberOfRelationInCategory"),
           FunctionTemplate::New(current, isMemberOfRelationInCategory)->GetFunction(context).ToLocalChecked());
  obj->Set(context, toV8("isMemberOfRelationWithTagKey"),
           FunctionTemplate::New(current, isMemberOfRelationWithTagKey)->GetFunction(context).ToLocalChecked());
  obj->Set(context, toV8("getNumRelationMemberNodes"),
           FunctionTemplate::New(current, getNumRelationMemberNodes)->GetFunction(context).ToLocalChecked());
  obj->Set(context, toV8("relationsHaveConnectedWayMembers"),
           FunctionTemplate::New(current, relationsHaveConnectedWayMembers)->GetFunction(context).ToLocalChecked());
}

void RelationMemberUtilsJs::isMemberOfRelationWithType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementId childId = toCpp<ElementId>(args[1]);
  QString relationType = toCpp<QString>(args[2]);

  const bool inRelationOfSpecifiedType =
    RelationMemberUtils::isMemberOfRelationWithType(map, childId, relationType);

  args.GetReturnValue().Set(Boolean::New(current, inRelationOfSpecifiedType));
}

void RelationMemberUtilsJs::isMemberOfRelationInCategory(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementId childId = toCpp<ElementId>(args[1]);
  QString schemaCategory = toCpp<QString>(args[2]);

  const bool inRelationOfSpecifiedCategory =
    RelationMemberUtils::isMemberOfRelationInCategory(map, childId, schemaCategory);
  LOG_VART(inRelationOfSpecifiedCategory);

  args.GetReturnValue().Set(Boolean::New(current, inRelationOfSpecifiedCategory));
}

void RelationMemberUtilsJs::isMemberOfRelationWithTagKey(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementId childId = toCpp<ElementId>(args[1]);
  QString tagKey = toCpp<QString>(args[2]);

  const bool inRelationWithSpecifiedTagKey =
    RelationMemberUtils::isMemberOfRelationWithTagKey(map, childId, tagKey);

  args.GetReturnValue().Set(Boolean::New(current, inRelationWithSpecifiedTagKey));
}

void RelationMemberUtilsJs::getNumRelationMemberNodes(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  ElementId relationId = toCpp<ElementId>(args[1]);
  ConstRelationPtr relation = map->getRelation(relationId.getId());
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
  ElementId relationId1 = toCpp<ElementId>(args[1]);
  ElementId relationId2 = toCpp<ElementId>(args[2]);
  if (relationId1.getType() != ElementType::Relation ||
      relationId2.getType() != ElementType::Relation)
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
          map->getRelation(relationId1.getId()), map->getRelation(relationId2.getId()))));
}

}
