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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SublineStringMatcherJs.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/algorithms/MultiLineStringSplitter.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/algorithms/linearreference/WaySublineMatchStringJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(SublineStringMatcherJs)

SublineStringMatcherJs::SublineStringMatcherJs(SublineStringMatcherPtr sm) : _sm(sm)
{
}

SublineStringMatcherJs::~SublineStringMatcherJs()
{
}

Handle<Value> SublineStringMatcherJs::extractMatchingSublines(const Arguments& args)
{
  HandleScope scope;

  SublineStringMatcherJs* smJs = ObjectWrap::Unwrap<SublineStringMatcherJs>(args.This());
  SublineStringMatcherPtr sm = smJs->getSublineStringMatcher();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ElementJs* e1Js = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject());
  ElementJs* e2Js = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject());
  ConstOsmMapPtr m = mapJs->getConstMap();
  ConstElementPtr e1 = e1Js->getConstElement();
  ConstElementPtr e2 = e2Js->getConstElement();

  Handle<Value> result;

  try
  {
    WaySublineMatchString match = sm->findMatch(m, e1, e2);

    if (match.isEmpty())
    {
      return Undefined();
    }

    // convert match into elements in a new map.
    set<ElementId> eids;
    eids.insert(e1->getElementId());
    eids.insert(e2->getElementId());
    OsmMapPtr copiedMap(new OsmMap(m->getProjection()));
    CopySubsetOp(m, eids).apply(copiedMap);
    WaySublineMatchString copiedMatch(match, copiedMap);

    // split the shared line based on the matching subline
    ElementPtr match1, scraps1;
    ElementPtr match2, scraps2;
    WaySublineCollection string1 = copiedMatch.getSublineString1();
    WaySublineCollection string2 = copiedMatch.getSublineString2();

    try
    {
      MultiLineStringSplitter().split(copiedMap, string1, copiedMatch.getReverseVector1(), match1,
        scraps1);
      MultiLineStringSplitter().split(copiedMap, string2, copiedMatch.getReverseVector2(), match2,
        scraps2);
    }
    catch (const IllegalArgumentException& e)
    {
      // this is unusual print out some information useful to debugging.
      MapProjector::projectToWgs84(copiedMap);
      LOG_WARN(OsmWriter::toString(copiedMap));
      throw e;
    }

    if (!match1 || !match2)
    {
      result = Undefined();
    }
    else
    {
      Handle<Object> obj = Object::New();
      obj->Set(String::NewSymbol("map"), OsmMapJs::create(copiedMap));
      obj->Set(String::NewSymbol("match1"), ElementJs::New(match1));
      obj->Set(String::NewSymbol("match2"), ElementJs::New(match2));
      result = obj;
    }
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }

  return scope.Close(result);
}

Handle<Value> SublineStringMatcherJs::findMatch(const Arguments& args)
{
  HandleScope scope;

  SublineStringMatcherJs* smJs = ObjectWrap::Unwrap<SublineStringMatcherJs>(args.This());

  if (args.Length() != 3)
  {
    throw IllegalArgumentException("Expected exactly three argument in findMatch (map, e1, e2)");
  }

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  ElementJs* e1Js = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject());
  ElementJs* e2Js = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject());

  WaySublineMatchString match = smJs->getSublineStringMatcher()->findMatch(
        mapJs->getConstMap(),
        e1Js->getConstElement(),
        e2Js->getConstElement());

  WaySublineMatchStringPtr result(new WaySublineMatchString(match));

  return scope.Close(WaySublineMatchStringJs::New(result));
}

void SublineStringMatcherJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(SublineStringMatcher::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("extractMatchingSublines"),
        FunctionTemplate::New(extractMatchingSublines)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("findMatch"),
        FunctionTemplate::New(findMatch)->GetFunction());

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> SublineStringMatcherJs::New(const Arguments& args) {
  HandleScope scope;

  QString className = str(args.This()->GetConstructorName());

  SublineStringMatcherPtr sm(
    Factory::getInstance().constructObject<SublineStringMatcher>(className));
  SublineStringMatcherJs* obj = new SublineStringMatcherJs(sm);
  PopulateConsumersJs::populateConsumers(sm.get(), args);
  obj->Wrap(args.This());

  return args.This();
}

}

