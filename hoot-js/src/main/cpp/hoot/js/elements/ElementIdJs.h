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
#ifndef __ELEMENT_ID_JS_H__
#define __ELEMENT_ID_JS_H__

// hoot
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

namespace hoot
{
using namespace std;
using namespace v8;

class OsmMapOperation;

/**
 *
 */
class ElementIdJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  ElementId& getElementId() { return _eid; }

  static Handle<Object> New(ElementId eid);

private:
  ElementIdJs();
  ~ElementIdJs();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  ElementId _eid;
  static Persistent<Function> _constructor;

  static Handle<Value> getType(const Arguments& args);
  static Handle<Value> toJSON(const Arguments& args);
  static Handle<Value> toString(const Arguments& args);
};

inline void toCpp(v8::Handle<v8::Value> v, ElementId& eid)
{
  if (v.IsEmpty() || !v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toString(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);

  QString className = str(obj->Get(PopulateConsumersJs::baseClass()));
  ElementIdJs* eidj = 0;
  if (obj->InternalFieldCount() >= 1 && className == QString::fromStdString(ElementId::className()))
  {
    eidj = node::ObjectWrap::Unwrap<ElementIdJs>(obj);
  }

  if (eidj)
  {
    eid = eidj->getElementId();
  }
  else if (obj->Has(String::NewSymbol("id")) && obj->Has(String::NewSymbol("type")))
  {
    long id;
    QString type;
    toCpp(obj->Get(String::NewSymbol("id")), id);
    toCpp(obj->Get(String::NewSymbol("type")), type);
    eid = ElementId(ElementType::fromString(type), id);
  }
  else
  {
    throw IllegalArgumentException("Expected an ElementId object, got: (" + toString(v) + ")");
  }
}

inline v8::Handle<v8::Value> toV8(const ElementId& eid)
{
  return ElementIdJs::New(eid);
}

}
#endif // __ELEMENT_ID_JS_H__
