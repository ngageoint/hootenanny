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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RelationMergerJs.h"

// hoot
#include <hoot/core/conflate/merging/RelationMerger.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(RelationMergerJs)

void RelationMergerJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> thisObj = Object::New(current);
  exports->Set(context, toV8("RelationMerger"), thisObj);

  thisObj->Set(
    context, toV8("merge"),
    FunctionTemplate::New(current, merge)->GetFunction(context).ToLocalChecked());
}

void RelationMergerJs::merge(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapPtr map = toCpp<OsmMapPtr>(args[0]);
  ElementId eid1 = toCpp<ElementId>(args[1]);
  ElementId eid2 = toCpp<ElementId>(args[2]);

  RelationMerger merger;
  merger.setOsmMap(map.get());
  try
  {
    merger.merge(eid1, eid2);
    args.GetReturnValue().SetUndefined();
  }
  catch (const HootException& err)
  {
    LOG_VARE(err.getWhat());
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

}
