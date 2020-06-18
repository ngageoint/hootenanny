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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiMatchDistanceJs.h"

// hoot
#include <hoot/core/conflate/point/PoiMatchDistance.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>

// Qt
#include <QString>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PoiMatchDistanceJs)

void PoiMatchDistanceJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> thisObj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "PoiMatchDistance"), thisObj);
  thisObj->Set(String::NewFromUtf8(current, "getMatchDistances"),
               FunctionTemplate::New(current, getMatchDistances)->GetFunction());
}

void PoiMatchDistanceJs::getMatchDistances(const FunctionCallbackInfo<Value>& /*args*/)
{
  ConfigOptions opts;
  QString matchDistancesData = opts.getPoiMatchDistances().trimmed();
  if (matchDistancesData.isEmpty())
  {
    matchDistancesData = opts.getPoiMatchDistancesConfig().trimmed();
  }
  if (matchDistancesData.isEmpty())
  {
    throw IllegalArgumentException(
      "Either a POI matches distances file must be specified with " +
      ConfigOptions::getPoiMatchDistancesConfigKey() + " or custom JSON specified with " +
      ConfigOptions::getPoiMatchDistancesKey() + ".");
  }

//  Handle<Array> distances =
//    Handle<Array>::Cast(toV8(PoiMatchDistance::readDistances(matchDistancesData)));
//  args.GetReturnValue().Set(distances);
}

}
