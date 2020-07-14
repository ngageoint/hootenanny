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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMSCHEMA_JS_H
#define OSMSCHEMA_JS_H

#include <hoot/core/schema/SchemaVertex.h>

// node.js
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/io/DataConvertJs.h>

namespace hoot
{

class OsmSchemaJs : public HootBaseJs
{
public:

  static void Init(v8::Handle<v8::Object> target);

  virtual ~OsmSchemaJs() = default;

private:

  OsmSchemaJs() = default;

  static void getAllTags(const v8::FunctionCallbackInfo<v8::Value>& agrs);
  static void getCategories(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getChildTagsAsVertices(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getSimilarTagsAsVertices(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getTagByCategory(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getTagVertex(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isAncestor(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isGeneric(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void hasType(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void explicitTypeMismatch(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void mostSpecificType(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void score(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void scoreTypes(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void scoreOneWay(const v8::FunctionCallbackInfo<v8::Value>& args);

  // Even thoughthe logic for some of these methods have been moved from OsmSchema to criterion,
  // decided to leave this interface intact, as it will be simpler to access from js.

  // TODO: All of these methods can go away if #3047 is completed.
  static void isArea(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isPoint(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isPolygon(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isBuilding(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isLinear(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isLinearWaterway(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isPowerLine(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isMetaData(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isPoi(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isRailway(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isHighway(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isNonBuildingArea(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void isCollectionRelation(const v8::FunctionCallbackInfo<v8::Value>& args);

  /**
   * See description in rules/HootLib.js isSpecificallyConflatable method
   */
  static void isSpecificallyConflatable(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void hasName(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void isLongRiverPair(const v8::FunctionCallbackInfo<v8::Value>& args);
};

inline v8::Handle<v8::Value> toV8(const SchemaVertex& tv)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::Handle<v8::Object> result = v8::Object::New(current);

  if (tv.isEmpty())
  {
    return v8::Undefined(current);
  }
  else
  {
    result->Set(toV8("name"), toV8(tv.name));
    result->Set(toV8("description"), toV8(tv.description));
    result->Set(toV8("key"), toV8(tv.key));
    result->Set(toV8("value"), toV8(tv.value));
    result->Set(toV8("influence"), toV8(tv.influence));
    result->Set(toV8("childWeight"), toV8(tv.childWeight));
    result->Set(toV8("mismatchScore"), toV8(tv.mismatchScore));
    // need to create a string conversion for this if we want to use it. Unused for now.
    //result->Set(toV8("valueType"), toV8(tv.valueType), None);
    result->Set(toV8("aliases"), toV8(tv.aliases));
    result->Set(toV8("categories"), toV8(tv.categories));
    result->Set(toV8("geometries"), toV8(tv.geometries));
  }

  return result;
}

}


#endif // OSMSCHEMA_JS_H
