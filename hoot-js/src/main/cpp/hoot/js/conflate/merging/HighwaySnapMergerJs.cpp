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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwaySnapMergerJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/highway/HighwayTagOnlyMerger.h>
#include <hoot/core/conflate/river/RiverSnapMerger.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/algorithms/subline-matching//SublineStringMatcherJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QStringList>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(HighwaySnapMergerJs)

Persistent<Function> HighwaySnapMergerJs::_constructor;

void HighwaySnapMergerJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, HighwaySnapMerger::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::NewFromUtf8(current, MergerBase::className().data()));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "apply"),
      FunctionTemplate::New(current, apply));

  _constructor.Reset(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, "HighwaySnapMerger"), ToLocal(&_constructor));
}

Handle<Object> HighwaySnapMergerJs::New(const HighwaySnapMergerPtr& ptr)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  HighwaySnapMergerJs* from = ObjectWrap::Unwrap<HighwaySnapMergerJs>(result);
  from->_ptr = ptr;

  return scope.Escape(result);
}

void HighwaySnapMergerJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  HighwaySnapMergerJs* obj = new HighwaySnapMergerJs();
  // node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void HighwaySnapMergerJs::apply(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  SublineStringMatcherPtr sublineMatcher = toCpp<SublineStringMatcherPtr>(args[0]);
  OsmMapPtr map = toCpp<OsmMapPtr>(args[1]);
  MergerBase::PairsSet pairs = toCpp<MergerBase::PairsSet>(args[2]);
  vector<pair<ElementId, ElementId>> replaced =
    toCpp<vector<pair<ElementId, ElementId>>>(args[3]);
  const QString matchedBy = toCpp<QString>(args[4]);
  SublineStringMatcherPtr sublineMatcher2;
  if (args.Length() > 5)
  {
    // This is little unusual, but we're allowing an extra subline matcher to be passed info for
    // river conflation only and the actual one used will be determined based on the input data.
    // See RiverSnapMerger for more info.
    if (matchedBy != "Waterway")
    {
      throw IllegalArgumentException(
        "Only river merging allows passing in multiple subline matchers.");
    }
    sublineMatcher2 = toCpp<SublineStringMatcherPtr>(args[5]);
  }

  // see explanation in ConflateCmd as to why we use this option to identify Attribute Conflation
  const bool isAttributeConflate = ConfigOptions().getHighwayMergeTagsOnly();
  HighwaySnapMergerPtr snapMerger;
  if (isAttributeConflate)
  {
    // HighwayTagOnlyMerger inherits from HighwaySnapMerger, so this works.
    snapMerger.reset(new HighwayTagOnlyMerger(pairs, sublineMatcher));
  }
  else if (matchedBy == "Waterway")
  {
    // see note above about multiple subline matchers here
    snapMerger.reset(new RiverSnapMerger(pairs, sublineMatcher, sublineMatcher2));
  }
  else
  {
    snapMerger.reset(new HighwaySnapMerger(pairs, sublineMatcher));
  }
  snapMerger->setMatchedBy(matchedBy);
  // Some attempts were made to use cached subline matches from SublineStringMatcherJst for
  // performance reasons, but the results were unstable. See branch 3969b.
  snapMerger->apply(map, replaced);

  // modify the parameter that was passed in
  Handle<Array> newArr = Handle<Array>::Cast(toV8(replaced));
  Handle<Array> arr = Handle<Array>::Cast(args[3]);
  arr->Set(String::NewFromUtf8(current, "length"), Integer::New(current, newArr->Length()));
  for (uint32_t i = 0; i < newArr->Length(); i++)
  {
    arr->Set(i, newArr->Get(i));
  }

  args.GetReturnValue().SetUndefined();
}

}

