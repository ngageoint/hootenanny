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
#ifndef __SUBLINE_STRING_MATCHER_JS_H__
#define __SUBLINE_STRING_MATCHER_JS_H__

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/io/DataConvertJs.h>

// Qt
#include <QString>

namespace hoot
{

class SublineStringMatcherJs : public HootBaseJs
{
public:

  static QString className() { return "hoot::SublineStringMatcherJs"; }

  static int logWarnCount;

  static void Init(v8::Local<v8::Object> target);
  virtual ~SublineStringMatcherJs() = default;
  static v8::Local<v8::Object> New(const SublineStringMatcherPtr& sd);

  SublineStringMatcherPtr getSublineStringMatcher() const { return _sm; }

private:

  SublineStringMatcherJs(SublineStringMatcherPtr sm) : _sm(sm) { }

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void extractMatchingSublines(const v8::FunctionCallbackInfo<v8::Value>& args);

  static v8::Persistent<v8::Function> _constructor;
  SublineStringMatcherPtr _sm;
};

inline void toCpp(v8::Local<v8::Value> v, SublineStringMatcherPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);
  SublineStringMatcherJs* ptrj = node::ObjectWrap::Unwrap<SublineStringMatcherJs>(obj);
  ptr = ptrj->getSublineStringMatcher();
}

inline v8::Local<v8::Value> toV8(const SublineStringMatcherPtr& matcher)
{
  return SublineStringMatcherJs::New(matcher);
}

}

#endif // __SUBLINE_STRING_MATCHER_JS_H__
