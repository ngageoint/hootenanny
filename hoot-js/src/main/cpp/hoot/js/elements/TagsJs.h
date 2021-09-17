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
#ifndef TAGS_JS_H
#define TAGS_JS_H

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/js/HootBaseJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>

namespace hoot
{

/**
 * This wraps the tags object. Unlike most of the other wrappers there is no smart pointer to the
 * tags object. When the TagsJs object is created it deep copies the tags (in an efficient and lazy
 * way). At some point it may make sense to make all references to tags pointers, but that isn't
 * strictly necessary yet.
 */
class TagsJs : public HootBaseJs
{
public:

  ~TagsJs() override = default;

  static void Init(v8::Local<v8::Object> target);

  Tags& getTags() { return _tags; }

  static v8::Local<v8::Object> New(const Tags& t);

private:

  Tags _tags;
  static v8::Persistent<v8::Function> _constructor;

  TagsJs() = default;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void contains(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void onlyOneContainsKvp(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void get(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getFirstMatchingKey(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getInformationCount(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getNames(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void set(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toDict(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void toString(const v8::FunctionCallbackInfo<v8::Value>& args);

  void _setTags(const Tags& t) { _tags = t; }
};

inline void toCpp(v8::Local<v8::Value> v, Tags& t)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);
  TagsJs* js = nullptr;
  if (obj->InternalFieldCount() > 0)
  {
    js = node::ObjectWrap::Unwrap<TagsJs>(obj);
  }
  if (js)
  {
    t = js->getTags();
  }
}

}
#endif // TAGS_JS_H
