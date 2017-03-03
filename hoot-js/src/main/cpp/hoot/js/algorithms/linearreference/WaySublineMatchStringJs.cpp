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
#include "WaySublineMatchStringJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(WaySublineMatchStringJs)

Persistent<Function> WaySublineMatchStringJs::_constructor;

WaySublineMatchStringJs::WaySublineMatchStringJs()
{
}

WaySublineMatchStringJs::~WaySublineMatchStringJs()
{
}

void WaySublineMatchStringJs::Init(Handle<Object> target)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol(WaySublineMatchString::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::New(WaySublineMatchString::className().data()));
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
      FunctionTemplate::New(toString)->GetFunction());

  _constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("WaySublineMatchString"), _constructor);
}

Handle<Value> WaySublineMatchStringJs::New(const Arguments& args)
{
  HandleScope scope;

  WaySublineMatchStringJs* obj = new WaySublineMatchStringJs();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Object> WaySublineMatchStringJs::New(WaySublineMatchStringPtr sm)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  WaySublineMatchStringJs* from = ObjectWrap::Unwrap<WaySublineMatchStringJs>(result);
  from->_sm = sm;

  return scope.Close(result);
}

Handle<Value> WaySublineMatchStringJs::toString(const Arguments& args)
{
  HandleScope scope;

  WaySublineMatchStringPtr sm =
    ObjectWrap::Unwrap<WaySublineMatchStringJs>(args.This())->getWaySublineMatchString();

  return scope.Close(String::New(sm->toString().toUtf8().data()));
}

}

