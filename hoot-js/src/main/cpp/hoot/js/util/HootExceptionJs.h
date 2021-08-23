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
#ifndef HOOTEXCEPTIONJS_H
#define HOOTEXCEPTIONJS_H

// hoot
#include <hoot/js/HootBaseJs.h>

namespace hoot
{

class HootExceptionJs : public HootBaseJs
{
public:

  static void Init(v8::Local<v8::Object> target);

  static v8::Local<v8::Object> create(const HootException& e) { return create(std::shared_ptr<HootException>(e.clone())); }
  static v8::Local<v8::Object> create(const std::shared_ptr<HootException>& e);

  std::shared_ptr<HootException> getException() const { return _e; }

  static bool isHootException(v8::Local<v8::Value> v);

  /**
   * A convenience function for checking the result of a V8 call. This will throw an appropriate
   * HootException if the result is empty.
   *
   * @param result Result of the V8 function call.
   * @param tc Try catch object. Must be instantiated before the V8 function is called.
   */
  static void checkV8Exception(v8::Local<v8::Value> result, const v8::TryCatch& tc);

  /**
   * This will throw an appropriate HootException based on the contents of tc.
   *
   * @param tc Try catch object. Must be instantiated before the V8 function is called.
   */
  static void throwAsHootException(const v8::TryCatch& tc);

  virtual ~HootExceptionJs() = default;

private:

  HootExceptionJs() = default;

  std::shared_ptr<HootException> _e;
  QString _className;
  static v8::Persistent<v8::Function> _constructor;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);
};

inline void toCpp(v8::Local<v8::Value> v, std::shared_ptr<HootException>& e)
{
  if (HootExceptionJs::isHootException(v))
  {
    v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);
    const HootExceptionJs* ex = node::ObjectWrap::Unwrap<HootExceptionJs>(obj);

    e = ex->getException();
  }
  else
  {
    throw IllegalArgumentException("Expected a HootExceptionJs, but got: " + toString(v));
  }
}

}

#endif // HOOTEXCEPTIONJS_H
