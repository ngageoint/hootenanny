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
#include "ElementVisitorJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementVisitorJs)

void ElementVisitorJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(ElementVisitor::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::NewFromUtf8(current, ElementVisitor::className().data()));

    Persistent<Function> constructor(current, tpl->GetFunction());
    target->Set(String::NewFromUtf8(current, n), ToLocal(&constructor));
  }
}

void ElementVisitorJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  const QString className = str(args.This()->GetConstructorName());
  LOG_VART(className);
  ElementVisitor* vis = Factory::getInstance().constructObject<ElementVisitor>(className);
  ElementVisitorJs* obj = new ElementVisitorJs(vis);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<ElementVisitor>(vis, args);

  args.GetReturnValue().Set(args.This());
}

}

