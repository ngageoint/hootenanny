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
#include "TagDifferencerJs.h"

// boost
#include <boost/any.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/core/schema/TagDifferencer.h>

// Qt
#include <QStringList>

using namespace boost;
using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(TagDifferencerJs)

TagDifferencerJs::TagDifferencerJs(TagDifferencer* td) : _td(td)
{
}

TagDifferencerJs::~TagDifferencerJs()
{
}

void TagDifferencerJs::diff(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope scope(current);

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

    args.GetReturnValue().Set(toV8(d));
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void TagDifferencerJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(TagDifferencer::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "diff"),
        FunctionTemplate::New(current, diff));
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::NewFromUtf8(current, TagDifferencer::className().data()));

    Persistent<Function> constructor(current, tpl->GetFunction());
    target->Set(String::NewFromUtf8(current, n), ToLocal(&constructor));
  }
}

void TagDifferencerJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    QString className = str(args.This()->GetConstructorName());
    if (className == "Object")
    {
      args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(IllegalArgumentException(
        "Invalid TagDifferencer. Did you forget 'new'?"))));
    }
    else
    {
      TagDifferencer* op = Factory::getInstance().constructObject<TagDifferencer>(className);
      TagDifferencerJs* obj = new TagDifferencerJs(op);
      //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
      obj->Wrap(args.This());

      PopulateConsumersJs::populateConsumers<TagDifferencer>(op, args);

      args.GetReturnValue().Set(args.This());
    }
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}

