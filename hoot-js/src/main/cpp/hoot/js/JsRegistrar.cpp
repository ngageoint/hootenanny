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
#include "JsRegistrar.h"

#include <hoot/core/Hoot.h>

namespace hoot
{

Handle<Value> Method(const Arguments& /*args*/) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

JsRegistrar* JsRegistrar::_theInstance = 0;

JsRegistrar::JsRegistrar()
{
}

JsRegistrar& JsRegistrar::getInstance()
{
  if (_theInstance == 0)
  {
    _theInstance = new JsRegistrar();
  }
  return *_theInstance;
}

void JsRegistrar::Init(Handle<Object> exports)
{
  Hoot::getInstance().init();
  getInstance().initAll(exports);
}

void JsRegistrar::initAll(Handle<Object> exports)
{
  exports->Set(String::NewSymbol("hello"),
      FunctionTemplate::New(Method)->GetFunction());

  for (size_t i = 0; i < _initializers.size(); i++)
  {
    _initializers[i]->Init(exports);
  }
}

void JsRegistrar::registerInitializer(boost::shared_ptr<ClassInitializer> ci)
{
  _initializers.push_back(ci);
}

}
