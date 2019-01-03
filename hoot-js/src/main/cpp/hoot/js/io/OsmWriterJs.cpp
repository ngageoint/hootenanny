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
#include "OsmWriterJs.h"

// hoot
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmWriterJs)

OsmWriterJs::OsmWriterJs()
{
}

OsmWriterJs::~OsmWriterJs() {}

void OsmWriterJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> writer = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "OsmWriter"), writer);
  writer->Set(String::NewFromUtf8(current, "toString"),
              FunctionTemplate::New(current, toString)->GetFunction());
}

void OsmWriterJs::toString(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[0]);
  bool formatXml = true;
  if (args.Length() > 1)
  {
    formatXml = toCpp<bool>(args[1]);
  }

  args.GetReturnValue().Set(toV8(OsmXmlWriter::toString(map, formatXml)));
}

}
