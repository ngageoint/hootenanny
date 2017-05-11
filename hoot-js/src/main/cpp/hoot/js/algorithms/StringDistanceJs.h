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

#ifndef STRINGDISTANCEJS_H
#define STRINGDISTANCEJS_H

// hoot
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/js/util/DataConvertJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

class OsmMapOperation;

/**
 *
 */
class StringDistanceJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  StringDistancePtr getStringDistance() { return _sd; }

  static v8::Handle<v8::Object> New(const StringDistancePtr& sd);

private:
  StringDistanceJs();
  StringDistanceJs(StringDistancePtr sd) { _sd = sd; }
  ~StringDistanceJs();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  StringDistancePtr _sd;
  static v8::Persistent<v8::Function> _constructor;

  static v8::Handle<v8::Value> toString(const v8::Arguments& args);
};

inline void toCpp(v8::Handle<v8::Value> v, StringDistancePtr& p)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  StringDistanceJs* sdj = 0;
  sdj = node::ObjectWrap::Unwrap<StringDistanceJs>(obj);
  if (sdj)
  {
    p = sdj->getStringDistance();
  }
  else
  {
    throw IllegalArgumentException("Expected a StringDistanceJs, got: (" + toJson(v) + ")");
  }
}

inline v8::Handle<v8::Value> toV8(const StringDistancePtr& sd)
{
  return StringDistanceJs::New(sd);
}

}

#endif // STRINGDISTANCEJS_H
