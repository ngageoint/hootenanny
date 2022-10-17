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
#include "FeatureExtractorJs.h"

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(FeatureExtractorJs)

FeatureExtractorJs::FeatureExtractorJs(FeatureExtractorPtr fe)
  : _fe(fe)
{
}

void FeatureExtractorJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  vector<QString> opNames = Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());

  for (auto& name : opNames)
  {
    QByteArray utf8 = name.replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, _new);
    tpl->SetClassName(
      String::NewFromUtf8(current, name.toStdString().data()).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(current, "extract", FunctionTemplate::New(current, _extract));

    Persistent<Function> constructor(current, tpl->GetFunction(context).ToLocalChecked());
    target->Set(context, toV8(n), ToLocal(&constructor));
  }
}

void FeatureExtractorJs::_extract(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  const FeatureExtractorJs* feJs = ObjectWrap::Unwrap<FeatureExtractorJs>(args.This());

  if (args.Length() != 3)
    throw IllegalArgumentException("Expected exactly three argument in extract (map, e1, e2)");

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  const ElementJs* e1Js = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked());
  const ElementJs* e2Js = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject(context).ToLocalChecked());

  double result =
    feJs->getFeatureExtractor()->extract(
      *(mapJs->getConstMap()), e1Js->getConstElement(), e2Js->getConstElement());

  if (result == FeatureExtractor::nullValue())
    args.GetReturnValue().SetNull();
  else
    args.GetReturnValue().Set(Number::New(current, result));
}

void FeatureExtractorJs::_new(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(v8::Isolate::GetCurrent());

  const QString className = "hoot::" + str(args.This()->GetConstructorName());

  FeatureExtractorPtr fe = Factory::getInstance().constructObject<FeatureExtractor>(className);
  FeatureExtractorJs* obj = new FeatureExtractorJs(fe);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<FeatureExtractor>(fe, args);

  args.GetReturnValue().Set(args.This());
}

}

