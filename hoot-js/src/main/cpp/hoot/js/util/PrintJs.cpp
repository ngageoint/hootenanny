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
#include "PrintJs.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/DataConvertJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PrintJs)

PrintJs::PrintJs()
{
}

PrintJs::~PrintJs() {}

void PrintJs::Init(Handle<Object> exports)
{
  exports->Set(String::NewSymbol("print"), FunctionTemplate::New(jsPrint)->GetFunction());
}

Handle<Value> PrintJs::jsPrint(const Arguments& args) {
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());

  QString result;

  for (int i = 0; i < args.Length(); i++)
  {
    if (i > 0)
    {
      result.append(" ");
    }

    result.append(toCpp<QString>(args[i]));
  }

  cout << result.toUtf8().data() << endl;

  return scope.Close(Undefined());
}

}
