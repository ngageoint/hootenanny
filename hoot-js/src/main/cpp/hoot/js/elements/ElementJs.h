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
#ifndef ELEMENTJS_H
#define ELEMENTJS_H

// Hoot
#include <hoot/core/elements/Element.h>

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
 * The ElementJs class wraps objects of type Element. It is an abstract base class.
 *
 * This class is a bit interesting in that it exposes non-const methods if it is wrapping a non-
 * const object, otherwise it exposes only const methods.
 */
class ElementJs : public node::ObjectWrap
{
public:
  virtual ConstElementPtr getConstElement() const = 0;
  virtual ElementPtr getElement() = 0;

  static Handle<Object> New(ConstElementPtr e);
  static Handle<Object> New(ElementPtr e);

protected:
  ElementJs();
  virtual ~ElementJs();
  static void _addBaseFunctions(Local<FunctionTemplate> tpl);

  static v8::Handle<v8::Value> getCircularError(const v8::Arguments& args);
  static v8::Handle<v8::Value> getElementId(const v8::Arguments& args);
  static v8::Handle<v8::Value> getId(const v8::Arguments& args);
  static v8::Handle<v8::Value> getStatusString(const v8::Arguments& args);
  static v8::Handle<v8::Value> getTags(const v8::Arguments& args);
  static v8::Handle<v8::Value> setStatusString(const v8::Arguments& args);
  static v8::Handle<v8::Value> setTags(const v8::Arguments& args);
  static v8::Handle<v8::Value> toString(const v8::Arguments& args);

private:
  QString _className;
  static Persistent<Function> _constructor;
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
