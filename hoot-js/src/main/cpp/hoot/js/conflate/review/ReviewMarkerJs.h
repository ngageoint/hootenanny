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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REVIEWMARKERJS_H
#define REVIEWMARKERJS_H

// hoot
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/io/DataConvertJs.h>

namespace hoot
{

/**
 * The ReviewMarkerJs class wraps objects of type ReviewMarker. It is an abstract base class.
 *
 * This class is a bit interesting in that it exposes non-const methods if it is wrapping a non-
 * const object, otherwise it exposes only const methods.
 */
class ReviewMarkerJs : public HootBaseJs
{
public:

  ~ReviewMarkerJs() override = default;

  static void Init(v8::Local<v8::Object> target);

private:

  ReviewMarkerJs() = default;

  static void mark(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif // REVIEWMARKERJS_H
