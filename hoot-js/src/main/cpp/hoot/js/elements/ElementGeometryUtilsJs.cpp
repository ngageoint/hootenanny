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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ElementGeometryUtilsJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h> 
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/elements/MapProjector.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementGeometryUtilsJs)

void ElementGeometryUtilsJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> thisObj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "ElementGeometryUtils"), thisObj);
  thisObj->Set(String::NewFromUtf8(current, "calculateLength"),
               FunctionTemplate::New(current, calculateLength)->GetFunction());
//  thisObj->Set(String::NewFromUtf8(current, "haveGeometricRelationship"),
//               FunctionTemplate::New(current, haveGeometricRelationship)->GetFunction());
}

void ElementGeometryUtilsJs::calculateLength(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
 
  try
  {
    Context::Scope context_scope(current->GetCurrentContext());

    ConstOsmMapPtr m = toCpp<ConstOsmMapPtr>(args[0]);
    ConstElementPtr e = toCpp<ConstElementPtr>(args[1]);

    args.GetReturnValue().Set(toV8(ElementGeometryUtils::calculateLength(e, m)));
  }
  catch (const HootException& err)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

//void ElementGeometryUtilsJs::haveGeometricRelationship(const FunctionCallbackInfo<Value>& args)
//{
//  Isolate* current = args.GetIsolate();
//  HandleScope scope(current);

//  try
//  {
//    Context::Scope context_scope(current->GetCurrentContext());

//    ConstElementPtr element = toCpp<ConstElementPtr>(args[0]);
//    std::shared_ptr<geos::geom::Geometry> bounds =
//      GeometryUtils::boundsFromString(toCpp<QString>(args[1]));
//    const GeometricRelationship relationship =
//      GeometricRelationship::fromString(toCpp<QString>(args[2]));
//    ConstOsmMapPtr map = toCpp<ConstOsmMapPtr>(args[3]);

//    if (!MapProjector::isGeographic(map))
//    {
//      // The bounds is always in WGS84, so if our map isn't currently in WGS84 we need to reproject
//      // the bounds.
//      std::shared_ptr<OGRSpatialReference> srs84(new OGRSpatialReference());
//      srs84->SetWellKnownGeogCS("WGS84");
//      MapProjector::project(bounds, srs84, map->getProjection());
//      LOG_VART(bounds);
//    }

//    args.GetReturnValue().Set(
//      Boolean::New(
//        current,
//        ElementGeometryUtils::haveGeometricRelationship(element, bounds, relationship, map)));
//  }
//  catch (const HootException& err)
//  {
//    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
//  }
//}

}

