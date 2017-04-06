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
#ifndef OSMMAPJS_H
#define OSMMAPJS_H

#include <hoot/js/SystemNodeJs.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/IdGeneratorJs.h>

namespace hoot
{
using namespace v8;

class OsmMapJs : public node::ObjectWrap
{
public:
 static void Init(v8::Handle<v8::Object> target);

 static Handle<Object> create(ConstOsmMapPtr map);
 static Handle<Object> create(OsmMapPtr map);

 OsmMapPtr& getMap();
 ConstOsmMapPtr& getConstMap() { return _constMap; }

 bool isConst() const { return !_map.get() && _constMap.get(); }

private:
 OsmMapJs();
 OsmMapJs(OsmMapPtr map);
 ~OsmMapJs();

  static Handle<Value> clone(const Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> getElement(const v8::Arguments& args);
  static v8::Handle<v8::Value> getElementCount(const v8::Arguments& args);
  static v8::Handle<v8::Value> getParents(const v8::Arguments& args);
  static v8::Handle<v8::Value> removeElement(const v8::Arguments& args);
  static v8::Handle<v8::Value> setIdGenerator(const v8::Arguments& args);
  static Handle<Value> visit(const Arguments& args);

  OsmMapPtr _map;
  ConstOsmMapPtr _constMap;
  static v8::Persistent<v8::Function> _constructor;

  void _setMap(OsmMapPtr map) { _map = map; _constMap = map; }
  void _setMap(ConstOsmMapPtr map) { _map.reset(); _constMap = map; }
};

inline void toCpp(v8::Handle<v8::Value> v, ConstOsmMapPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  OsmMapJs* ptrj = node::ObjectWrap::Unwrap<OsmMapJs>(obj);
  ptr = ptrj->getConstMap();
}

inline void toCpp(v8::Handle<v8::Value> v, OsmMapPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  OsmMapJs* ptrj = node::ObjectWrap::Unwrap<OsmMapJs>(obj);
  ptr = ptrj->getMap();
}

}

#endif // OSMMAPJS_H
