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
#ifndef HOOTEXCEPTIONJS_H
#define HOOTEXCEPTIONJS_H

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace node;
using namespace v8;

class HootExceptionJs : public node::ObjectWrap
{
public:
  static void Init(Handle<Object> target);

  static Handle<Object> create(const HootException& e) { return create(boost::shared_ptr<HootException>(e.clone())); }
  static Handle<Object> create(boost::shared_ptr<HootException> e);

 boost::shared_ptr<HootException> getException() const { return _e; }

  static bool isHootException(Handle<Value> v);

  /**
   * A convenience function for checking the result of a V8 call. This will throw an appropriate
   * HootException if the result is empty.
   *
   * @param result Result of the V8 function call.
   * @param tc Try catch object. Must be instantiated before the V8 function is called.
   */
  static void checkV8Exception(Handle<Value> result, TryCatch& tc);

  /**
   * This will throw an appropriate HootException based on the contents of tc.
   *
   * @param tc Try catch object. Must be instantiated before the V8 function is called.
   */
  static void throwAsHootException(TryCatch& tc);

private:
  HootExceptionJs();

 boost::shared_ptr<HootException> _e;
  QString _className;
  static Persistent<Function> _constructor;

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> toJSON(const v8::Arguments& args);
  static v8::Handle<v8::Value> toString(const v8::Arguments& args);

};

inline void toCpp(v8::Handle<v8::Value> v,boost::shared_ptr<HootException>& e)
{
  if (HootExceptionJs::isHootException(v))
  {
    v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
    HootExceptionJs* ex = node::ObjectWrap::Unwrap<HootExceptionJs>(obj);

    e = ex->getException();
  }
  else
  {
    throw IllegalArgumentException("Expected a HootExceptionJs, but got: " + toString(v));
  }
}

}

#endif // HOOTEXCEPTIONJS_H
