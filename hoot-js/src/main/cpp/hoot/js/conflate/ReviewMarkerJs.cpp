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
#define BUILDING_NODE_EXTENSION
#include "ReviewMarkerJs.h"

// hoot
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/visitors/ElementVisitorJs.h>
#include <hoot/js/visitors/JsFunctionVisitor.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include "../elements/ElementJs.h"
#include "../OsmMapJs.h"

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ReviewMarkerJs)

ReviewMarkerJs::ReviewMarkerJs() {}

ReviewMarkerJs::~ReviewMarkerJs() {}

void ReviewMarkerJs::Init(Handle<Object> exports)
{
  Handle<Object> reviewMarker = Object::New();
  exports->Set(String::NewSymbol("ReviewMarker"), reviewMarker);
  reviewMarker->Set(String::NewSymbol("mark"), FunctionTemplate::New(mark)->GetFunction());
}

Handle<Value> ReviewMarkerJs::mark(const Arguments& args)
{
  HandleScope scope;

  OsmMapPtr osmMap = toCpp<OsmMapPtr>(args[0]);

  ElementPtr e = toCpp<ElementPtr>(args[1]);

  QString note = toCpp<QString>(args[2]);

  QString reviewType = toCpp<QString>(args[3]);

  double score = toCpp<double>(args[4]);

  vector<QString> choices = toCpp< vector<QString> >(args[5]);

  ReviewMarker::mark(osmMap, e, note, reviewType, score, choices);

  return scope.Close(Undefined());
}

}



