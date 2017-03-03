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
#ifndef __SUBLINE_STRING_MATCHER_JS_H__
#define __SUBLINE_STRING_MATCHER_JS_H__

// hoot
#include <hoot/core/algorithms/SublineStringMatcher.h>
#include <hoot/js/util/DataConvertJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

namespace hoot
{
using namespace std;
using namespace v8;

class OsmMapOperation;

class SublineStringMatcherJs : public node::ObjectWrap
{
public:

  static std::string className() { return "hoot::SublineStringMatcherJs"; }

  static unsigned int logWarnCount;

  static void Init(v8::Handle<v8::Object> target);

  SublineStringMatcherPtr getSublineStringMatcher() { return _sm; }

private:

  SublineStringMatcherJs(SublineStringMatcherPtr sm);
  ~SublineStringMatcherJs();

  static v8::Handle<v8::Value> extractMatchingSublines(const v8::Arguments& args);
  static v8::Handle<v8::Value> findMatch(const v8::Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  QString _className;
  SublineStringMatcherPtr _sm;
};

inline void toCpp(v8::Handle<v8::Value> v, SublineStringMatcherPtr& ptr)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  SublineStringMatcherJs* ptrj = node::ObjectWrap::Unwrap<SublineStringMatcherJs>(obj);
  ptr = ptrj->getSublineStringMatcher();
}

}

#endif // __SUBLINE_STRING_MATCHER_JS_H__
