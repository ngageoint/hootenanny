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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "LinearMergerJs.h"

// hoot
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/algorithms/subline-matching/SublineStringMatcherJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/SystemNodeJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(LinearMergerJs)

Persistent<Function> LinearMergerJs::_constructor;

void LinearMergerJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  const QString name = "LinearMerger";
  tpl->SetClassName(String::NewFromUtf8(current, name.toStdString().data()).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(), toV8(MergerBase::className()));
  tpl->PrototypeTemplate()->Set(current, "apply", FunctionTemplate::New(current, apply));

  _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
  Maybe<bool> success = target->Set(context, toV8(name), ToLocal(&_constructor));
  (void) success; // unused variable
}

void LinearMergerJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  LinearMergerJs* obj = new LinearMergerJs();
  // node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void LinearMergerJs::apply(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  SublineStringMatcherPtr sublineStringMatcher = toCpp<SublineStringMatcherPtr>(args[0]);
  // POTENTIAL BUG HERE: This subline string matcher has already been configured when
  // SublineStringMatcherFactory creates it during matching. However, if we don't call
  // setConfiguration here, js linear conflation performs worse on some data due to extra match
  // conflicts being recognized during match optimization. What's happening here is that, due to the
  // manner in which MaximalSublineStringMatcher::setConfiguration is set up, the configured subline
  // matchers are being ignored and reset to MaximalNearestSublineMatcher when setConfiguration is
  // called here. That isn't really a good thing due to the confusion it causes, but the only other
  // options would be to either add a separate subline matcher for each data type to be used during
  // match conflict resolution (and possibly also during merging) only (cumbersome), or just
  // hardcode the configuration of the subline string matcher here to always use
  // MaximalNearestSublineMatcher as its primary matcher (not as cumbersome, but more brittle). So,
  // staying with this behavior for the time being. Eventually, it may be worth correcting this
  // issue.
  sublineStringMatcher->setConfiguration(conf());
  LOG_VART(sublineStringMatcher->getName());
  LOG_VART(sublineStringMatcher->getSublineMatcherName());
  OsmMapPtr map = toCpp<OsmMapPtr>(args[1]);
  MergerBase::PairsSet pairs = toCpp<MergerBase::PairsSet>(args[2]);
  vector<pair<ElementId, ElementId>> replaced = toCpp<vector<pair<ElementId, ElementId>>>(args[3]);
  const QString matchedBy = toCpp<QString>(args[4]);
  LOG_VART(matchedBy);

  MergerPtr merger = LinearMergerFactory::getMerger(pairs, sublineStringMatcher, matchedBy);
  LOG_VART(merger->getClassName());
  merger->apply(map, replaced);

  // Modify the parameter that was passed in.
  Local<Array> newArr = Local<Array>::Cast(toV8(replaced));
  Local<Array> arr = Local<Array>::Cast(args[3]);
  Maybe<bool> success = arr->Set(context, toV8(MetadataTags::Length()), Integer::New(current, newArr->Length()));
  for (uint32_t i = 0; i < newArr->Length(); i++)
    success = arr->Set(context, i, newArr->Get(context, i).ToLocalChecked());

  args.GetReturnValue().SetUndefined();
}

}

