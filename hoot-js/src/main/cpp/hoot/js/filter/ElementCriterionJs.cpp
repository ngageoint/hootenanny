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
#include "ElementCriterionJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementCriterionJs)

ElementCriterionJs::ElementCriterionJs(ElementCriterion* c) : _c(c)
{
}

ElementCriterionJs::~ElementCriterionJs()
{
}

Handle<Value> ElementCriterionJs::addCriterion(const Arguments& args) {
  HandleScope scope;

  ElementCriterionPtr addTo = ObjectWrap::Unwrap<ElementCriterionJs>(args.This())->getCriterion();

  ElementCriterionPtr other = ObjectWrap::Unwrap<ElementCriterionJs>(args[0]->ToObject())->
      getCriterion();

 boost::shared_ptr<ElementCriterionConsumer> consumer =
      dynamic_pointer_cast<ElementCriterionConsumer>(addTo);

  if (!consumer)
  {
    consumer->addCriterion(other);
  }
  else
  {
    ThrowException(Exception::TypeError(String::New("This criterion doesn't support adding "
                                                    "criterion.")));
    return scope.Close(Undefined());
  }

  return scope.Close(Undefined());
}

void ElementCriterionJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("addCriterion"),
        FunctionTemplate::New(addCriterion)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("isSatisfied"),
        FunctionTemplate::New(isSatisfied)->GetFunction());
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::New(ElementCriterion::className().data()));

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> ElementCriterionJs::isSatisfied(const v8::Arguments& args)
{
  HandleScope scope;

  ElementCriterionPtr ec = ObjectWrap::Unwrap<ElementCriterionJs>(args.This())->getCriterion();

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args[0]->ToObject())->getConstElement();

  return scope.Close(Boolean::New(ec->isSatisfied(e)));
}

Handle<Value> ElementCriterionJs::New(const Arguments& args) {
  HandleScope scope;

  QString className = str(args.This()->GetConstructorName());

  ElementCriterion* c = Factory::getInstance().constructObject<ElementCriterion>(className);
  ElementCriterionJs* obj = new ElementCriterionJs(c);
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<ElementCriterion>(c, args);

  return args.This();
}

}

