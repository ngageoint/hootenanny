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

#ifndef MATCHFACTORYJS_H
#define MATCHFACTORYJS_H

// node.js
#include <hoot/core/conflate/MatchCreator.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/util/DataConvertJs.h>

namespace hoot
{

class MatchFactoryJs : public node::ObjectWrap
{
public:
 static void Init(v8::Handle<v8::Object> target);

private:
  MatchFactoryJs();
  ~MatchFactoryJs();

  static void getAllAvailableCreators(const v8::FunctionCallbackInfo<v8::Value>& args);
};

inline v8::Handle<v8::Value> toV8(const MatchCreator::Description& d)
{
  v8::Handle<v8::Object> result = v8::Object::New(v8::Isolate::GetCurrent());
  result->Set(toV8("className"), toV8(d.className));
  result->Set(toV8("description"), toV8(d.description));
  result->Set(toV8("experimental"), toV8(d.experimental));
  return result;
}

}

#endif // MATCHFACTORYJS_H
