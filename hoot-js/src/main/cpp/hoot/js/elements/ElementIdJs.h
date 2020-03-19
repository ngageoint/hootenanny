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
#ifndef __ELEMENT_ID_JS_H__
#define __ELEMENT_ID_JS_H__

// hoot
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

namespace hoot
{

/**
 *
 */
class ElementIdJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  ElementId& getElementId() { return _eid; }

  static v8::Handle<v8::Object> New(ElementId eid);

private:
  ElementIdJs();
  ~ElementIdJs();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  ElementId _eid;
  static v8::Persistent<v8::Function> _constructor;

  static void getType(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toJSON(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);
};

inline void toCpp(v8::Handle<v8::Value> v, ElementId& eid)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
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
  else if (obj->Has(v8::String::NewFromUtf8(current, "id")) &&
           obj->Has(v8::String::NewFromUtf8(current, "type")))
  {
    long id;
    QString type;
    toCpp(obj->Get(v8::String::NewFromUtf8(current, "id")), id);
    toCpp(obj->Get(v8::String::NewFromUtf8(current, "type")), type);
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
