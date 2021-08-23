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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "SublineStringMatcherJs.h"

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/Factory.h>

#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt

#include <QElapsedTimer>

using namespace std;
using namespace v8;

namespace hoot
{

int SublineStringMatcherJs::logWarnCount = 0;
Persistent<Function> SublineStringMatcherJs::_constructor;

HOOT_JS_REGISTER(SublineStringMatcherJs)

void SublineStringMatcherJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  vector<QString> opNames =
    Factory::getInstance().getObjectNamesByBase(SublineStringMatcher::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = opNames[i].replace("hoot::", "").toUtf8();
    const char* n = utf8.data();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(String::NewFromUtf8(current, opNames[i].toStdString().data()).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(current, "extractMatchingSublines",
        FunctionTemplate::New(current, extractMatchingSublines));

    _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
    target->Set(context, toV8(n), ToLocal(&_constructor));
  }
}

void SublineStringMatcherJs::New(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  const QString className = "hoot::" + str(args.This()->GetConstructorName());

  SublineStringMatcherPtr sm =
    Factory::getInstance().constructObject<SublineStringMatcher>(className);
  SublineStringMatcherJs* obj = new SublineStringMatcherJs(sm);
  PopulateConsumersJs::populateConsumers(sm, args);
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

Local<Object> SublineStringMatcherJs::New(const SublineStringMatcherPtr& matcher)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  SublineStringMatcherJs* from = ObjectWrap::Unwrap<SublineStringMatcherJs>(result);
  from->_sm = matcher;

  return scope.Escape(result);
}

void SublineStringMatcherJs::extractMatchingSublines(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  const SublineStringMatcherJs* smJs = ObjectWrap::Unwrap<SublineStringMatcherJs>(args.This());
  SublineStringMatcherPtr sm = smJs->getSublineStringMatcher();

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject(context).ToLocalChecked());
  const ElementJs* e1Js = ObjectWrap::Unwrap<ElementJs>(args[1]->ToObject(context).ToLocalChecked());
  const ElementJs* e2Js = ObjectWrap::Unwrap<ElementJs>(args[2]->ToObject(context).ToLocalChecked());
  ConstOsmMapPtr m = mapJs->getConstMap();
  ConstElementPtr e1 = e1Js->getConstElement();
  ConstElementPtr e2 = e2Js->getConstElement();

  Local<Value> result;
  try
  {
    // Some attempts were made to use cached subline matches here from SublineStringMatcherJs for
    // performance reasons, but the results were unstable. Doing so could lead to a runtime
    // performance boost, so worth revisiting. See branch 3969b.
    WaySublineMatchString match = sm->findMatch(m, e1, e2);

    if (match.isEmpty() || !match.isValid())
    {
      LOG_TRACE("Empty or invalid match");
      args.GetReturnValue().SetUndefined();
      return;
    }

    // convert match into elements in a new map
    set<ElementId> eids;
    eids.insert(e1->getElementId());
    eids.insert(e2->getElementId());
    OsmMapPtr copiedMap = std::make_shared<OsmMap>(m->getProjection());
    CopyMapSubsetOp(m, eids).apply(copiedMap);
    LOG_VART(copiedMap->size());
    WaySublineMatchString copiedMatch(match, copiedMap);

    // split the shared line based on the matching subline
    ElementPtr match1, scraps1;
    ElementPtr match2, scraps2;
    WaySublineCollection string1 = copiedMatch.getSublineString1();
    LOG_VART(string1);
    WaySublineCollection string2 = copiedMatch.getSublineString2();
    LOG_VART(string2);

    MultiLineStringSplitter splitter;
    try
    {
      splitter.split(copiedMap, string1, copiedMatch.getReverseVector1(), match1, scraps1);
      splitter.split(copiedMap, string2, copiedMatch.getReverseVector2(), match2, scraps2);
    }
    catch (const IllegalArgumentException&)
    {
      MapProjector::projectToWgs84(copiedMap);
      LOG_TRACE(OsmXmlWriter::toString(copiedMap));
      logWarnCount++;
      throw;
    }

    if (!match1 || !match2)
    {
      LOG_TRACE("No match");
      result = Undefined(current);
    }
    else
    {
      LOG_TRACE("match");
      Local<Object> obj = Object::New(current);
      obj->Set(context, toV8("map"), OsmMapJs::create(copiedMap));
      obj->Set(context, toV8("match1"), ElementJs::New(match1));
      obj->Set(context, toV8("match2"), ElementJs::New(match2));
      result = obj;
    }
    args.GetReturnValue().Set(result);
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}

