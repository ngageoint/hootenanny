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
#ifndef HIGHWAYSNAPMERGERJS_H
#define HIGHWAYSNAPMERGERJS_H

// hoot
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/js/util/DataConvertJs.h>

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
class HighwaySnapMergerJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  HighwaySnapMergerPtr getHighwaySnapMerger() { return _ptr; }

  static Handle<Object> New(const HighwaySnapMergerPtr& ptr);

private:
  HighwaySnapMergerJs();
  ~HighwaySnapMergerJs();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static Handle<Value> apply(const Arguments& args);

  HighwaySnapMergerPtr _ptr;
  static Persistent<Function> _constructor;

};

inline void toCpp(v8::Handle<v8::Value> v, HighwaySnapMergerPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  HighwaySnapMergerJs* ptrj = node::ObjectWrap::Unwrap<HighwaySnapMergerJs>(obj);
  ptr = ptrj->getHighwaySnapMerger();
}

}

#endif // HIGHWAYSNAPMERGERJS_H
