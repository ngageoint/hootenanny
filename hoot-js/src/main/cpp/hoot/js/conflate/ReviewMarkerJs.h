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
#ifndef REVIEWMARKERJS_H
#define REVIEWMARKERJS_H

// #include <nodejs/node.h>
#include <hoot/js/SystemNodeJs.h>

// hoot
#include <hoot/js/util/DataConvertJs.h>

namespace hoot
{

using namespace node;
using namespace v8;

/**
 * The ReviewMarkerJs class wraps objects of type ReviewMarker. It is an abstract base class.
 *
 * This class is a bit interesting in that it exposes non-const methods if it is wrapping a non-
 * const object, otherwise it exposes only const methods.
 */
class ReviewMarkerJs : public node::ObjectWrap
{
public:

  static void Init(v8::Handle<v8::Object> target);

private:
  ReviewMarkerJs();
  ~ReviewMarkerJs();

  static v8::Handle<v8::Value> mark(const v8::Arguments& args);
};

}

#endif // REVIEWMARKERJS_H
