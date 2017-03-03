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
#ifndef TAGS_JS_H
#define TAGS_JS_H

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/DataConvertJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

namespace hoot
{
using namespace std;
using namespace v8;

class OsmMapOperation;

/**
 * This wraps the tags object. Unlike most of the other wrappers there is no smart pointer to the
 * tags object. When the TagsJs object is created it deep copies the tags (in an efficient and lazy
 * way). At some point it may make sense to make all references to tags pointers, but that isn't
 * strictly necessary yet.
 */
class TagsJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  Tags& getTags() { return _tags; }

  static Handle<Object> New(const Tags& t);

private:
  TagsJs(ConstNodePtr n);
  TagsJs();
  ~TagsJs();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  Tags _tags;
  static Persistent<Function> _constructor;

  static Handle<Value> contains(const Arguments& args);
  static Handle<Value> get(const Arguments& args);
  static Handle<Value> getCreateUuid(const Arguments& args);
  static Handle<Value> getLengthInMeters(const Arguments& args);
  static Handle<Value> getVelocityInMeters(const Arguments& args);
  static Handle<Value> getInformationCount(const Arguments& args);
  static Handle<Value> set(const Arguments& args);
  static Handle<Value> toDict(const Arguments& args);
  static Handle<Value> toString(const Arguments& args);

  void _setTags(const Tags& t) { _tags = t; }
};

inline void toCpp(v8::Handle<v8::Value> v, Tags& t)
{
  if (!v->IsObject())
  {
    throw IllegalArgumentException("Expected an object, got: (" + toJson(v) + ")");
  }

  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
  TagsJs* js = 0;
  if (obj->InternalFieldCount() > 0)
  {
    js = node::ObjectWrap::Unwrap<TagsJs>(obj);
  }
  if (js)
  {
    t = js->getTags();
  }
  else
  {
    QStringList keys = toCpp<QStringList>(obj->GetPropertyNames());

    if (keys.size() == 0)
    {
      throw IllegalArgumentException("Expected a MostEnglishNameJs, got: (" + toJson(v) + ")");
    }
    else
    {
      t.clear();
      for (int i = 0; i < keys.size(); i++)
      {
        QString value = toCpp<QString>(obj->Get(toV8(keys[i])));
        t[keys[i]] = value;
      }
    }
  }
}

}
#endif // TAGS_JS_H
