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
#include "ElementConverterJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h> 

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementConverterJs)

ElementConverterJs::ElementConverterJs()
{
}

ElementConverterJs::~ElementConverterJs() {}

void ElementConverterJs::Init(Handle<Object> exports)
{
  Handle<Object> thisObj = Object::New();
  exports->Set(String::NewSymbol("ElementConverter"), thisObj);
  thisObj->Set(String::NewSymbol("calculateLength"), FunctionTemplate::New(calculateLength)->
               GetFunction());
}

Handle<Value> ElementConverterJs::calculateLength(const Arguments& args) {
  HandleScope scope;
 
  try
  {
    Context::Scope context_scope(Context::GetCurrent());

  	ConstOsmMapPtr m = toCpp<ConstOsmMapPtr>(args[0]);
  	ConstElementPtr e = toCpp<ConstElementPtr>(args[1]);

    return scope.Close(toV8(ElementConverter(m).calculateLength(e)));
  }
  catch ( const HootException& err )
  {
    LOG_VAR(err.getWhat());
    return v8::ThrowException(HootExceptionJs::create(err));
  }

  return scope.Close(Undefined());
}

}

