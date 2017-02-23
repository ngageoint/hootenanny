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
#ifndef OSMSCHEMA_JS_H
#define OSMSCHEMA_JS_H

#include <hoot/core/schema/SchemaVertex.h>

// node.js
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/util/DataConvertJs.h>

namespace hoot
{
using namespace node;
using namespace v8;

class OsmSchemaJs : public node::ObjectWrap
{
public:
 static void Init(v8::Handle<v8::Object> target);

private:
  OsmSchemaJs();
  ~OsmSchemaJs();

  static v8::Handle<v8::Value> getAllTags(const v8::Arguments&);
  static v8::Handle<v8::Value> getCategories(const v8::Arguments& args);
  static v8::Handle<v8::Value> getChildTags(const v8::Arguments& args);
  static v8::Handle<v8::Value> getSimilarTags(const v8::Arguments& args);
  static v8::Handle<v8::Value> getTagByCategory(const v8::Arguments& args);
  static v8::Handle<v8::Value> getTagVertex(const v8::Arguments& args);
  static v8::Handle<v8::Value> isAncestor(const v8::Arguments& args);
  static v8::Handle<v8::Value> isArea(const v8::Arguments& args);
  static v8::Handle<v8::Value> isBuilding(const v8::Arguments& args);
  static v8::Handle<v8::Value> isHgisPoi(const v8::Arguments& args);
  static v8::Handle<v8::Value> isLinear(const v8::Arguments& args);
  static v8::Handle<v8::Value> isLinearWaterway(const v8::Arguments& args);
  static v8::Handle<v8::Value> isMetaData(const v8::Arguments& args);
  static v8::Handle<v8::Value> isPoi(const v8::Arguments& args);
  static v8::Handle<v8::Value> isLinearHighway(const v8::Arguments& args);
  static v8::Handle<v8::Value> score(const v8::Arguments& args);
  static v8::Handle<v8::Value> scoreOneWay(const v8::Arguments& args);

};

inline v8::Handle<v8::Value> toV8(const SchemaVertex& tv)
{
  v8::Handle<v8::Object> result = v8::Object::New();

  if (tv.isEmpty())
  {
    return v8::Undefined();
  }
  else
  {
    result->Set(toV8("name"), toV8(tv.name), None);
    result->Set(toV8("description"), toV8(tv.description), None);
    result->Set(toV8("key"), toV8(tv.key), None);
    result->Set(toV8("value"), toV8(tv.value), None);
    result->Set(toV8("influence"), toV8(tv.influence), None);
    result->Set(toV8("childWeight"), toV8(tv.childWeight), None);
    result->Set(toV8("mismatchScore"), toV8(tv.mismatchScore), None);
    // need to create a string conversion for this if we want to use it. Unused for now.
    //result->Set(toV8("valueType"), toV8(tv.valueType), None);
    result->Set(toV8("aliases"), toV8(tv.aliases), None);
    result->Set(toV8("categories"), toV8(tv.categories), None);
    result->Set(toV8("geometries"), toV8(tv.geometries), None);
  }

  return result;
}

}


#endif // OSMSCHEMA_JS_H
