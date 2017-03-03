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
#include "TagDifferencerJs.h"

// boost
#include <boost/any.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/core/schema/TagDifferencer.h>

// Qt
#include <QStringList>

namespace hoot
{

using namespace boost;

HOOT_JS_REGISTER(TagDifferencerJs)

TagDifferencerJs::TagDifferencerJs(TagDifferencer* td) : _td(td)
{
}

TagDifferencerJs::~TagDifferencerJs()
{
}

Handle<Value> TagDifferencerJs::diff(const Arguments& args) {
  HandleScope scope;

  try
  {
    TagDifferencerJs* op = ObjectWrap::Unwrap<TagDifferencerJs>(args.This());

    ConstOsmMapPtr& map = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject())->getConstMap();
    ConstElementPtr e1 = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject())->getConstElement();
    ConstElementPtr e2 = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject())->getConstElement();

    if (!map || !e1 || !e2)
    {
      throw IllegalArgumentException("You must specify a valid map, element 1 and element 2");
    }

    double d = op->getDifferencer()->diff(map, e1, e2);

    return scope.Close(toV8(d));
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

void TagDifferencerJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(TagDifferencer::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("diff"),
        FunctionTemplate::New(diff)->GetFunction());
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::New(TagDifferencer::className().data()));

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> TagDifferencerJs::New(const Arguments& args) {
  HandleScope scope;

  try
  {
    QString className = str(args.This()->GetConstructorName());
    if (className == "Object")
    {
      return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Invalid TagDifferencer. Did you forget 'new'?")));
    }
    TagDifferencer* op = Factory::getInstance().constructObject<TagDifferencer>(className);
    TagDifferencerJs* obj = new TagDifferencerJs(op);
    obj->Wrap(args.This());

    PopulateConsumersJs::populateConsumers<TagDifferencer>(op, args);

    return args.This();
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

}

