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
#include "MostEnglishNameJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(MostEnglishNameJs)

Persistent<Function> MostEnglishNameJs::_constructor;

void MostEnglishNameJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  QString name = QString::fromStdString(MostEnglishName::className()).replace("hoot::", "");
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, MostEnglishName::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getMostEnglishName"),
      FunctionTemplate::New(current, getMostEnglishName));
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                String::NewFromUtf8(current, MostEnglishName::className().data()));

  Persistent<Function> constructor(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, name.toUtf8().data()), ToLocal(&constructor));
}

void MostEnglishNameJs::New(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  MostEnglishName* c = new MostEnglishName();
  MostEnglishNameJs* obj = new MostEnglishNameJs(MostEnglishNamePtr(c));
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<MostEnglishName>(c, args);

  args.GetReturnValue().Set(args.This());
}

void MostEnglishNameJs::getMostEnglishName(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(v8::Isolate::GetCurrent());

  MostEnglishNamePtr sd = toCpp<MostEnglishNamePtr>(args.This());
  Tags t = toCpp<Tags>(args[0]);

  args.GetReturnValue().Set(toV8(sd->getMostEnglishName(t)));
}

}

