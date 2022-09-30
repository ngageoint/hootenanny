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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ELEMENT_ID_JS_H__
#define __ELEMENT_ID_JS_H__

// hoot
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>

namespace hoot
{

class ElementIdJs : public HootBaseJs
{
public:

  ~ElementIdJs() override = default;

  static void Init(v8::Local<v8::Object> target);
  static v8::Local<v8::Object> New(ElementId eid);

  ElementId& getElementId() { return _eid; }

private:

  ElementIdJs() = default;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  ElementId _eid;
  static v8::Persistent<v8::Function> _constructor;

  static void getType(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);
};

inline void toCpp(v8::Local<v8::Value> v, ElementId& eid)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();

  // try as string first
  if (v->IsString())
  {
    eid = ElementId(toCpp<QString>(v));
    return;
  }

  // now try as an object
  if (v.IsEmpty() || !v->IsObject())
    throw IllegalArgumentException("Expected an object, got: (" + toString(v) + ")");

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);

  QString className = str(obj->Get(context, PopulateConsumersJs::baseClass()).ToLocalChecked());
  ElementIdJs* eidj = nullptr;
  if (obj->InternalFieldCount() >= 1 && className == ElementId::className())
    eidj = node::ObjectWrap::Unwrap<ElementIdJs>(obj);

  if (eidj)
    eid = eidj->getElementId();
  else if (obj->Has(context, v8::String::NewFromUtf8(current, "id").ToLocalChecked()).ToChecked() &&
           obj->Has(context, v8::String::NewFromUtf8(current, "type").ToLocalChecked()).ToChecked())
  {
    long id;
    QString type;
    toCpp(obj->Get(context, toV8("id")).ToLocalChecked(), id);
    toCpp(obj->Get(context, toV8("type")).ToLocalChecked(), type);
    eid = ElementId(ElementType::fromString(type), id);
  }
  else
    throw IllegalArgumentException("Expected an ElementId object, got: (" + toString(v) + ")");
}

inline v8::Local<v8::Value> toV8(const ElementId& eid)
{
  return ElementIdJs::New(eid);
}

}
#endif // __ELEMENT_ID_JS_H__
