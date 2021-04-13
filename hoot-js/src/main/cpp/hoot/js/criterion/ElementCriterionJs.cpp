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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ElementCriterionJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementCriterionJs)

Persistent<Function> ElementCriterionJs::_constructor;

void ElementCriterionJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  vector<QString> opNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = opNames[i].replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, opNames[i].toStdString().data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(
      PopulateConsumersJs::baseClass(),
      String::NewFromUtf8(current, ElementCriterion::className().toStdString().data()));
    tpl->PrototypeTemplate()->Set(
      String::NewFromUtf8(current, "isSatisfied"), FunctionTemplate::New(current, isSatisfied));

    _constructor.Reset(current, tpl->GetFunction());
    target->Set(String::NewFromUtf8(current, n), ToLocal(&_constructor));
  }
}

void ElementCriterionJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  const QString className = "hoot::" + str(args.This()->GetConstructorName());
  LOG_VART(className);
  ElementCriterion* c = Factory::getInstance().constructObject<ElementCriterion>(className);
  ElementCriterionJs* obj = new ElementCriterionJs(c);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<ElementCriterion>(c, args);

  args.GetReturnValue().Set(args.This());
}

void ElementCriterionJs::isSatisfied(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ElementCriterionPtr ec = ObjectWrap::Unwrap<ElementCriterionJs>(args.This())->getCriterion();
  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  args.GetReturnValue().Set(Boolean::New(current, ec->isSatisfied(e)));
}

}

