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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FeatureExtractorJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(FeatureExtractorJs)

FeatureExtractorJs::FeatureExtractorJs(FeatureExtractorPtr fe) : _fe(fe)
{
}

FeatureExtractorJs::~FeatureExtractorJs()
{
}

Handle<Value> FeatureExtractorJs::extract(const Arguments& args) {
  HandleScope scope;

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
    return scope.Close(Null());
  }
  else
  {
    return scope.Close(Number::New(result));
  }
}

void FeatureExtractorJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("extract"),
        FunctionTemplate::New(extract)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("getName"),
        FunctionTemplate::New(getName)->GetFunction());

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> FeatureExtractorJs::New(const Arguments& args) {
  HandleScope scope;

  QString className = str(args.This()->GetConstructorName());

  FeatureExtractorPtr fe(Factory::getInstance().constructObject<FeatureExtractor>(className));
  FeatureExtractorJs* obj = new FeatureExtractorJs(fe);
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<FeatureExtractor>(fe.get(), args);

  return args.This();
}

Handle<Value> FeatureExtractorJs::getName(const Arguments& args) {
  HandleScope scope;

  return scope.Close(toV8(toCpp<FeatureExtractorPtr>(args.This())->getName()));
}

}

