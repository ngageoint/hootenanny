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

#ifndef VALUEAGGREGATORJS_H
#define VALUEAGGREGATORJS_H

// hoot
#include <hoot/core/algorithms/aggregator/ValueAggregator.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/HootBaseJs.h>

namespace hoot
{

/**
 *
 */
class ValueAggregatorJs : public HootBaseJs
{
public:

  static void Init(v8::Local<v8::Object> target);

  ValueAggregatorPtr getValueAggregator() { return _va; }

  static v8::Local<v8::Object> New(const ValueAggregatorPtr& va);

  virtual ~ValueAggregatorJs() = default;

private:

  ValueAggregatorJs(ValueAggregatorPtr va) { _va = va; }
  ValueAggregatorJs() = default;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  ValueAggregatorPtr _va;
  static v8::Persistent<v8::Function> _constructor;
};

inline void toCpp(v8::Local<v8::Value> v, ValueAggregatorPtr& p)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);
  ValueAggregatorJs* vaj = nullptr;
  vaj = node::ObjectWrap::Unwrap<ValueAggregatorJs>(obj);
  if (vaj)
  {
    p = vaj->getValueAggregator();
  }
  else
  {
    throw IllegalArgumentException("Expected a ValueAggregatorJs, got: (" + toJson(v) + ")");
  }
}

inline v8::Local<v8::Value> toV8(const ValueAggregatorPtr& va)
{
  return ValueAggregatorJs::New(va);
}

}

#endif // VALUEAGGREGATORJS_H
