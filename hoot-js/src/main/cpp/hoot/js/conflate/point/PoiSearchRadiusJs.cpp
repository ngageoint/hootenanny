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
#include "PoiSearchRadiusJs.h"

// hoot
#include <hoot/core/conflate/point/PoiSearchRadius.h>
#include <hoot/core/util/CollectionUtils.h>
#include <hoot/core/util/ConfigOptions.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>

// Qt


using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(PoiSearchRadiusJs)

void PoiSearchRadiusJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> thisObj = Object::New(current);
  exports->Set(context, toV8("PoiSearchRadius"), thisObj);
  thisObj->Set(context, toV8("getSearchRadii"),
               FunctionTemplate::New(current, getSearchRadii)->GetFunction(context).ToLocalChecked());
}

bool PoiSearchRadiusJs::_searchRadiiOptionIsConfigFile(const QString data)
{
  return
    data.toLower().endsWith(".json") && !data.trimmed().startsWith("{") &&
    !data.trimmed().endsWith("}");
}

bool PoiSearchRadiusJs::_searchRadiiOptionIsJsonString(const QString data)
{
  return
    !data.toLower().endsWith(".json") && data.trimmed().startsWith("{") &&
    data.trimmed().endsWith("}");
}

void PoiSearchRadiusJs::getSearchRadii(const FunctionCallbackInfo<Value>& args)
{
  const QString searchRadiiData = ConfigOptions().getPoiSearchRadii().trimmed();
  if (!_searchRadiiOptionIsConfigFile(searchRadiiData) &&
      !_searchRadiiOptionIsJsonString(searchRadiiData))
  {
    throw IllegalArgumentException(
      "Either a POI search radii config file or custom JSON must be specified by " +
      ConfigOptions::getPoiSearchRadiiKey() + ".");
  }

  const QList<PoiSearchRadius> searchRadiiList =
    PoiSearchRadius::readSearchRadii(searchRadiiData);

  Isolate* current = Isolate::GetCurrent();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Array> searchRadii = Array::New(current, searchRadiiList.size());
  searchRadii->Set(context, toV8("length"), Integer::New(current, searchRadiiList.size()));
  for (int i = 0 ; i < searchRadiiList.size(); i++)
  {
    const PoiSearchRadius searchRadius = searchRadiiList.at(i);
    LOG_VART(searchRadius);

    Local<Object> searchRadiusObj = Object::New(current);
    searchRadiusObj->Set(context, toV8("key"), toV8(searchRadius.getKey()));
    if (!searchRadius.getValue().isEmpty())
    {
      searchRadiusObj->Set(context, toV8("value"), toV8(searchRadius.getValue()));
    }
    searchRadiusObj->Set(context, toV8("distance"), toV8(searchRadius.getDistance()));

    searchRadii->Set(context, (uint32_t)i, searchRadiusObj);
  }
  args.GetReturnValue().Set(searchRadii);
}

}
