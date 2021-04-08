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
#ifndef LINEAR_SNAP_MERGER_JS_H
#define LINEAR_SNAP_MERGER_JS_H

// hoot
#include <hoot/core/conflate/merging/LinearSnapMerger.h>
#include <hoot/js/io/DataConvertJs.h>

// node.js
#include <hoot/js/HootBaseJs.h>

// Standard
#include <memory>

namespace hoot
{

/**
 * Facade around linear features mergers in core
 *
 * @todo This class should be renamed to LinearMergerJs.
 */
class LinearSnapMergerJs : public HootBaseJs
{
public:

  static void Init(v8::Handle<v8::Object> target);

  LinearSnapMergerPtr getLinearSnapMerger() { return _ptr; }

  virtual ~LinearSnapMergerJs() = default;

private:

  LinearSnapMergerJs() = default;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void apply(const v8::FunctionCallbackInfo<v8::Value>& args);

  LinearSnapMergerPtr _ptr;
  static v8::Persistent<v8::Function> _constructor;
};

inline void toCpp(v8::Handle<v8::Value> v, LinearSnapMergerPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  LinearSnapMergerJs* ptrj = node::ObjectWrap::Unwrap<LinearSnapMergerJs>(obj);
  ptr = ptrj->getLinearSnapMerger();
}

}

#endif // LINEAR_SNAP_MERGER_JS_H
