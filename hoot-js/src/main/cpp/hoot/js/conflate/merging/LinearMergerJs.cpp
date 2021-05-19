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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "LinearMergerJs.h"

// hoot
#include <hoot/core/conflate/merging/LinearAverageMerger.h>
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/conflate/merging/LinearTagOnlyMerger.h>
#include <hoot/core/util/Factory.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/algorithms/subline-matching//SublineStringMatcherJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/elements/OsmMapJs.h>
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
  target->Set(context, toV8(name), ToLocal(&_constructor));
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
    // This is little unusual, but we're allowing an extra subline matcher to be passed in for
    // certain conflation types. The general idea is that one matcher may be more accurate but
    // slower (e.g. maximal subline) and the other may be slightly less accurate but much
    // quicker (Frechet). The actual one used here will be determined based how the matcher
    // performs against the input data.
    if (matchedBy != "Waterway" && matchedBy != "Line")
    {
      throw IllegalArgumentException(
        "Only river or generic line merging allows passing in multiple subline matchers.");
    }
    sublineMatcher2 = toCpp<SublineStringMatcherPtr>(args[5]);
  }

  LOG_VART(ConfigOptions().getGeometryLinearMergerDefault());
  // Use of LinearTagOnlyMerger for geometries signifies that we're doing Attribute Conflation.
  const bool isAttributeConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearTagOnlyMerger::className();
  // Use of LinearAverageMerger for geometries signifies that we're doing Average Conflation.
  const bool isAverageConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearAverageMerger::className();
  MergerPtr merger;
  if (isAttributeConflate || isAverageConflate ||
      (matchedBy != "Waterway" && matchedBy != "Line") || !sublineMatcher2)
  {
    merger = LinearMergerFactory::getMerger(pairs, sublineMatcher, matchedBy);
  }
  else
  {
    merger = LinearMergerFactory::getMerger(pairs, sublineMatcher, sublineMatcher2, matchedBy);
  }
  LOG_VART(merger->getClassName());
  merger->apply(map, replaced);

  // Modify the parameter that was passed in.
  Local<Array> newArr = Local<Array>::Cast(toV8(replaced));
  Local<Array> arr = Local<Array>::Cast(args[3]);
  arr->Set(context, toV8("length"), Integer::New(current, newArr->Length()));
  for (uint32_t i = 0; i < newArr->Length(); i++)
  {
    arr->Set(context, i, newArr->Get(context, i).ToLocalChecked());
  }

  args.GetReturnValue().SetUndefined();
}

}

