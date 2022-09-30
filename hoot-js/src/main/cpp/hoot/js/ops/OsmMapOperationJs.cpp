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
#include "OsmMapOperationJs.h"

// boost
#include <boost/any.hpp>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmMapOperationJs)

OsmMapOperationJs::OsmMapOperationJs(std::shared_ptr<OsmMapOperation> op)
  : _op(op)
{
}

void OsmMapOperationJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  vector<QString> opNames = Factory::getInstance().getObjectNamesByBase(OsmMapOperation::className());

  for (const auto& name : opNames)
  {
    QByteArray utf8 = name.toUtf8();
    const char* n = utf8.data();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, name.toStdString().data()).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(current, "apply", FunctionTemplate::New(current, apply));
    tpl->PrototypeTemplate()->Set(current, "applyAndGetResult", FunctionTemplate::New(current, applyAndGetResult));
    tpl->PrototypeTemplate()->Set(
      PopulateConsumersJs::baseClass(),
      String::NewFromUtf8(current, OsmMapOperation::className().toStdString().data()).ToLocalChecked());

    Persistent<Function> constructor(current, tpl->GetFunction(context).ToLocalChecked());
    target->Set(context, toV8(n), ToLocal(&constructor));
  }
}

void OsmMapOperationJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  const QString className = str(args.This()->GetConstructorName());
  if (className == "Object")
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        HootExceptionJs::create(IllegalArgumentException("Invalid OsmMapOperation. Did you forget 'new'?"))));
  }
  std::shared_ptr<OsmMapOperation> op = Factory::getInstance().constructObject<OsmMapOperation>(className);
  OsmMapOperationJs* obj = new OsmMapOperationJs(op);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<OsmMapOperation>(op, args);

  args.GetReturnValue().Set(args.This());
}

void OsmMapOperationJs::apply(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapOperationJs* op = ObjectWrap::Unwrap<OsmMapOperationJs>(args.This());
  OsmMapPtr& map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked())->getMap();

  op->getMapOp()->apply(map);

  args.GetReturnValue().SetUndefined();
}

void OsmMapOperationJs::applyAndGetResult(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapOperationJs* op = ObjectWrap::Unwrap<OsmMapOperationJs>(args.This());
  OsmMapPtr& map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked())->getMap();
  op->getMapOp()->apply(map);
  boost::any result = op->getMapOp()->getResult();

  /// I'm sure there's a better way to do this...initially just supporting a small number of
  /// returned types here.
  if (result.type() == typeid(double))
    args.GetReturnValue().Set(Number::New(current, boost::any_cast<double>(result)));
  else if (result.type() == typeid(int))
    args.GetReturnValue().Set(Number::New(current, boost::any_cast<int>(result)));
  else if (result.type() == typeid(QString))
    args.GetReturnValue().Set(String::NewFromUtf8(current, boost::any_cast<QString>(result).toLatin1().data()).ToLocalChecked());
  else
    throw HootException("Unsupported OsmMapOperation result type encountered by Javascript API.");
}

}
