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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "LinearSnapMergerJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/linear/LinearTagOnlyMerger.h>
#include <hoot/core/conflate/MultipleSublineMatcherSnapMerger.h>

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

HOOT_JS_REGISTER(LinearSnapMergerJs)

Persistent<Function> LinearSnapMergerJs::_constructor;

void LinearSnapMergerJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(
     String::NewFromUtf8(current, LinearSnapMerger::className().toStdString().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::NewFromUtf8(current, MergerBase::className().toStdString().data()));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "apply"),
      FunctionTemplate::New(current, apply));

  _constructor.Reset(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, "LinearSnapMerger"), ToLocal(&_constructor));
}

Handle<Object> LinearSnapMergerJs::New(const LinearSnapMergerPtr& ptr)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  LinearSnapMergerJs* from = ObjectWrap::Unwrap<LinearSnapMergerJs>(result);
  from->_ptr = ptr;

  return scope.Escape(result);
}

void LinearSnapMergerJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  LinearSnapMergerJs* obj = new LinearSnapMergerJs();
  // node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void LinearSnapMergerJs::apply(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  SublineStringMatcherPtr sublineMatcher = toCpp<SublineStringMatcherPtr>(args[0]);
  OsmMapPtr map = toCpp<OsmMapPtr>(args[1]);
  MergerBase::PairsSet pairs = toCpp<MergerBase::PairsSet>(args[2]);
  vector<pair<ElementId, ElementId>> replaced =
    toCpp<vector<pair<ElementId, ElementId>>>(args[3]);
  const QString matchedBy = toCpp<QString>(args[4]);
  LOG_VART(matchedBy);
  SublineStringMatcherPtr sublineMatcher2;
  if (args.Length() > 5)
  {
    // This is little unusual, but we're allowing an extra subline matcher to be passed info for
    // certain conflation types, and the actual one used will be determined based how the matcher
    // performs against the input data. See WaySnapMerger for more info.
    if (matchedBy != "Waterway" && matchedBy != "Line")
    {
      throw IllegalArgumentException(
        "Only river or generic line merging allows passing in multiple subline matchers.");
    }
    sublineMatcher2 = toCpp<SublineStringMatcherPtr>(args[5]);
  }

  // see explanation in ConflateCmd as to why we use this option to identify Attribute Conflation
  const bool isAttributeConflate = ConfigOptions().getHighwayMergeTagsOnly();
  LinearSnapMergerPtr snapMerger;
  if (isAttributeConflate)
  {
    // LinearTagOnlyMerger inherits from LinearSnapMerger, so this works.
    snapMerger.reset(new LinearTagOnlyMerger(pairs, sublineMatcher));
  }
  else if (matchedBy == "Waterway" || matchedBy == "Line")
  {
    // see note above about multiple subline matchers here
    snapMerger.reset(new MultipleSublineMatcherSnapMerger(pairs, sublineMatcher, sublineMatcher2));
  }
  else
  {
    snapMerger.reset(new LinearSnapMerger(pairs, sublineMatcher));
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

