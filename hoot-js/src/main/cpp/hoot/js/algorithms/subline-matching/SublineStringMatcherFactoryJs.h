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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef _SUBLINE_STRING_MATCHER_FACTORY_JS_H_
#define _SUBLINE_STRING_MATCHER_FACTORY_JS_H_

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcherFactory.h>
#include <hoot/js/HootBaseJs.h>

namespace hoot
{

class SublineStringMatcherFactoryJs : public HootBaseJs
{
public:

  static QString className() { return "hoot::SublineStringMatcherFactoryJs"; }

  static void Init(v8::Local<v8::Object> target);
  virtual ~SublineStringMatcherFactoryJs() = default;

private:

  SublineStringMatcherFactoryJs() = default;

  static void getMatcher(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}

#endif // _SUBLINE_STRING_MATCHER_FACTORY_JS_H_
