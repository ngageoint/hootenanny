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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTJS_H
#define ELEMENTJS_H

// Hoot
#include <hoot/core/elements/Element.h>

// node.js
#include <hoot/js/HootBaseJs.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * The ElementJs class wraps objects of type Element. It is an abstract base class.
 *
 * This class is a bit interesting in that it exposes non-const methods if it is wrapping a non-
 * const object, otherwise it exposes only const methods.
 */
class ElementJs : public HootBaseJs
{
public:

  virtual ConstElementPtr getConstElement() const = 0;
  virtual ElementPtr getElement() = 0;

  static v8::Handle<v8::Object> New(ConstElementPtr e);
  static v8::Handle<v8::Object> New(ElementPtr e);

  virtual ~ElementJs() = default;

protected:

  ElementJs() = default;
  static void _addBaseFunctions(v8::Local<v8::FunctionTemplate> tpl);

  static void getCircularError(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getElementId(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getId(const v8::FunctionCallbackInfo<v8::Value>& args);
  /**
   * Return the zero-based input number associated with this element. If this is not an input, throw
   * an exception.
   */
  static void getStatusInput(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getStatusString(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getTags(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setStatusString(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setTags(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void setTag(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void removeTag(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);

private:

  static v8::Persistent<v8::Function> _constructor;
};

inline void toCpp(v8::Handle<v8::Value> v, ConstElementPtr& e)
{
  if (v.IsEmpty() || !v->IsObject())
  {
    throw IllegalArgumentException("Expected input to be an ElementJs object type.");
  }
  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  e = node::ObjectWrap::Unwrap<ElementJs>(obj)->getConstElement();
}

inline void toCpp(v8::Handle<v8::Value> v, ElementPtr& e)
{
  if (v.IsEmpty() || !v->IsObject())
  {
    throw IllegalArgumentException("Expected input to be an ElementJs object type.");
  }
  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  e = node::ObjectWrap::Unwrap<ElementJs>(obj)->getElement();
}

inline v8::Handle<v8::Value> toV8(const ConstElementPtr& e)
{
  return ElementJs::New(e);
}

inline v8::Handle<v8::Value> toV8(const ElementPtr& e)
{
  return ElementJs::New(e);
}

}

#endif // ELEMENTJS_H
