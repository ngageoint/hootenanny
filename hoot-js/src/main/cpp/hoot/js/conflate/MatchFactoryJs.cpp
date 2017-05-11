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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactoryJs.h"

// hoot
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/js/JsRegistrar.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(MatchFactoryJs)

MatchFactoryJs::MatchFactoryJs()
{
}

MatchFactoryJs::~MatchFactoryJs() {}

void MatchFactoryJs::Init(Handle<Object> exports)
{
  Handle<Object> schema = Object::New();
  exports->Set(String::NewSymbol("MatchFactory"), schema);
  schema->Set(String::NewSymbol("getAllAvailableCreators"),
    FunctionTemplate::New(getAllAvailableCreators)->GetFunction());
}

Handle<Value> MatchFactoryJs::getAllAvailableCreators(const Arguments& /*args*/)
{
  HandleScope scope;

  return scope.Close(toV8(MatchFactory::getInstance().getAllAvailableCreators()));
}

}
