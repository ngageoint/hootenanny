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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FeatureExtractorJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(FeatureExtractorJs)

FeatureExtractorJs::FeatureExtractorJs(FeatureExtractorPtr fe) : _fe(fe)
{
}

FeatureExtractorJs::~FeatureExtractorJs()
{
}

void FeatureExtractorJs::extract(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  FeatureExtractorJs* feJs = ObjectWrap::Unwrap<FeatureExtractorJs>(args.This());

  if (args.Length() != 3)
  {
    throw IllegalArgumentException("Expected exactly three argument in extract (map, e1, e2)");
  }

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ElementJs* e1Js = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject());
  ElementJs* e2Js = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject());

  double result = feJs->getFeatureExtractor()->extract(
        *(mapJs->getConstMap()),
        e1Js->getConstElement(),
        e2Js->getConstElement());

  if (result == feJs->getFeatureExtractor()->nullValue())
  {
    args.GetReturnValue().SetNull();
  }
  else
  {
    args.GetReturnValue().Set(Number::New(current, result));
  }
}

void FeatureExtractorJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "extract"),
        FunctionTemplate::New(current, extract));
    tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getName"),
        FunctionTemplate::New(current, getName));

    Persistent<Function> constructor(current, tpl->GetFunction());
    target->Set(String::NewFromUtf8(current, n), ToLocal(&constructor));
  }
}

void FeatureExtractorJs::New(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(v8::Isolate::GetCurrent());

  QString className = str(args.This()->GetConstructorName());

  FeatureExtractorPtr fe(Factory::getInstance().constructObject<FeatureExtractor>(className));
  FeatureExtractorJs* obj = new FeatureExtractorJs(fe);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<FeatureExtractor>(fe.get(), args);

  args.GetReturnValue().Set(args.This());
}

void FeatureExtractorJs::getName(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  args.GetReturnValue().Set(toV8(toCpp<FeatureExtractorPtr>(args.This())->getName()));
}

}

