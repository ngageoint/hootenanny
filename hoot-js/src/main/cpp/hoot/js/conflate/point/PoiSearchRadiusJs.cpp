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
#include "PoiSearchRadiusJs.h"

// hoot
#include <hoot/core/conflate/point/PoiSearchRadius.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>

// Qt
#include <QString>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PoiSearchRadiusJs)

void PoiSearchRadiusJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> thisObj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "PoiSearchRadius"), thisObj);
  thisObj->Set(String::NewFromUtf8(current, "getSearchRadii"),
               FunctionTemplate::New(current, getSearchRadii)->GetFunction());
}

void PoiSearchRadiusJs::getSearchRadii(const FunctionCallbackInfo<Value>& args)
{
  ConfigOptions opts;
  // This should be a JSON string, not a file.
  QString searchRadiiData = opts.getPoiSearchRadii().trimmed();
  if (searchRadiiData.toLower().endsWith(".json") && !searchRadiiData.trimmed().startsWith("{") &&
      !searchRadiiData.trimmed().endsWith("}"))
  {
    throw IllegalArgumentException("poi.search.radii should contain a JSON string.");
  }
  if (searchRadiiData.isEmpty())
  {
    // This should be a JSON file.
    searchRadiiData = opts.getPoiSearchRadiiConfig().trimmed();
    if (!searchRadiiData.toLower().endsWith(".json"))
    {
      throw IllegalArgumentException("poi.search.radii.config should point to a JSON file.");
    }
  }
  if (searchRadiiData.isEmpty())
  {
    throw IllegalArgumentException(
      "Either a POI search radii file must be specified by " +
      ConfigOptions::getPoiSearchRadiiConfigKey() + " or custom JSON specified by " +
      ConfigOptions::getPoiSearchRadiiKey() + ".");
  }

  const QList<PoiSearchRadius> searchRadiiList =
    PoiSearchRadius::readSearchRadii(searchRadiiData);

  Isolate* current = v8::Isolate::GetCurrent();
  Local<Array> searchRadii = Array::New(current, searchRadiiList.size());
  searchRadii->Set(
    String::NewFromUtf8(current, "length"), Integer::New(current, searchRadiiList.size()));
  for (int i = 0 ; i < searchRadiiList.size(); i++)
  {
    const PoiSearchRadius searchRadius = searchRadiiList.at(i);
    LOG_VART(searchRadius);

    Local<Object> searchRadiusObj = Object::New(current);
    searchRadiusObj->Set(toV8(QString("key")), toV8(searchRadius.getKey()));
    if (!searchRadius.getValue().isEmpty())
    {
      searchRadiusObj->Set(toV8(QString("value")), toV8(searchRadius.getValue()));
    }
    searchRadiusObj->Set(toV8(QString("distance")), toV8(searchRadius.getDistance()));

    searchRadii->Set((uint32_t)i, searchRadiusObj);
  }
  args.GetReturnValue().Set(searchRadii);
}

}
