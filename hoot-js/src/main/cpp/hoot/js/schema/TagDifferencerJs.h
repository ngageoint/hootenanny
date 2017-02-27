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
#ifndef __TAG_DIFFERENCER_JS_H__
#define __TAG_DIFFERENCER_JS_H__

// hoot
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Std
#include <memory>

namespace hoot
{
class TagDifferencer;

using namespace std;
using namespace v8;

class TagDifferencer;

class TagDifferencerJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  TagDifferencer* getDifferencer() { return _td.get(); }

private:
  TagDifferencerJs(TagDifferencer *op);
  ~TagDifferencerJs();

  static v8::Handle<v8::Value> diff(const v8::Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  QString _className;
  auto_ptr<TagDifferencer> _td;
};

}

#endif // __TAG_DIFFERENCER_JS_H__
