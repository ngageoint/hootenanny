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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef VISITORJS_H
#define VISITORJS_H

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

namespace hoot
{

class ElementVisitor;

class ElementVisitorJs : public node::ObjectWrap
{
public:

  static void Init(v8::Handle<v8::Object> target);

  std::shared_ptr<ElementVisitor> getVisitor() { return _v; }

private:

  ElementVisitorJs(ElementVisitor* v) : _v(v) {}
  ElementVisitorJs() {}
  ~ElementVisitorJs() {}

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  std::shared_ptr<ElementVisitor> _v;
};

}

#endif // VISITORJS_H
