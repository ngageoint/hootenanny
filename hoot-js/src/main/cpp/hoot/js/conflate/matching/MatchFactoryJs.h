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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef MATCHFACTORYJS_H
#define MATCHFACTORYJS_H

// hoot
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/io/DataConvertJs.h>

namespace hoot
{

class MatchFactoryJs : public HootBaseJs
{
public:

  ~MatchFactoryJs() override = default;

  static void Init(v8::Local<v8::Object> target);

private:

  MatchFactoryJs() = default;
};

inline v8::Local<v8::Value> toV8(const CreatorDescription& d)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Local<v8::Object> result = v8::Object::New(v8::Isolate::GetCurrent());
  result->Set(context, toV8("className"), toV8(d.getClassName()));
  result->Set(context, toV8("description"), toV8(d.getDescription()));
  result->Set(context, toV8("experimental"), toV8(d.getExperimental()));
  return result;
}

}

#endif // MATCHFACTORYJS_H
