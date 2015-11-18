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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __MOST_ENGLISH_NAME_JS_H__
#define __MOST_ENGLISH_NAME_JS_H__

// hoot
#include <hoot/core/algorithms/string/MostEnglishName.h>
#include <hoot/js/util/DataConvertJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{
using namespace std;
using namespace v8;

class OsmMapOperation;

/**
 *
 */
class MostEnglishNameJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  MostEnglishNamePtr getPtr() { return _sd; }

  static Handle<Object> New(const MostEnglishNamePtr& sd);

private:
  MostEnglishNameJs();
  MostEnglishNameJs(MostEnglishNamePtr sd) { _sd = sd; }
  ~MostEnglishNameJs();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  MostEnglishNamePtr _sd;
  static Persistent<Function> _constructor;

  static Handle<Value> getMostEnglishName(const Arguments& args);
};

inline void toCpp(v8::Handle<v8::Value> v, MostEnglishNamePtr& p)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  MostEnglishNameJs* js = 0;
  js = node::ObjectWrap::Unwrap<MostEnglishNameJs>(obj);
  if (js)
  {
    p = js->getPtr();
  }
  else
  {
    throw IllegalArgumentException("Expected a MostEnglishNameJs, got: (" + toJson(v) + ")");
  }
}

inline v8::Handle<v8::Value> toV8(const MostEnglishNamePtr& sd)
{
  return MostEnglishNameJs::New(sd);
}

}

#endif // __MOST_ENGLISH_NAME_JS_H__
