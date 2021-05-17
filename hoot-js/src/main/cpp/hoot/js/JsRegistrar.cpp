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
#include "JsRegistrar.h"

#include <hoot/core/Hoot.h>
#include <hoot/core/util/Log.h>

using namespace v8;

namespace hoot
{

void Method(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  args.GetReturnValue().Set(String::NewFromUtf8(current, "world"));
}

JsRegistrar& JsRegistrar::getInstance()
{
  //  Local static singleton instance
  static JsRegistrar instance;
  return instance;
}

void JsRegistrar::Init(Local<Object> exports)
{
  LOG_DEBUG("JS registrar init...");
  Hoot::getInstance().init();
  getInstance().initAll(exports);
}

void JsRegistrar::initAll(Local<Object> exports)
{
  // Got this from the NodeJS docs. Seems to be a bit simpler than what we were doing.
  NODE_SET_METHOD(exports, "hello", Method);

  for (size_t i = 0; i < _initializers.size(); i++)
  {
    _initializers[i]->Init(exports);
  }
}

void JsRegistrar::registerInitializer(const std::shared_ptr<ClassInitializer>& ci)
{
  _initializers.push_back(ci);
}

}
