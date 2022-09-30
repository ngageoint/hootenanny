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
#ifndef WAYJS_H
#define WAYJS_H

// hoot
#include <hoot/core/elements/Way.h>

#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

class WayJs : public ElementJs
{
public:

  static void Init(v8::Local<v8::Object> target);

  ConstElementPtr getConstElement() const override { return getConstWay(); }
  ConstWayPtr getConstWay() const { return _constWay; }
  ElementPtr getElement() override { return getWay(); }
  WayPtr getWay() const { assert(_way); return _way; }

  static v8::Local<v8::Object> New(ConstWayPtr way);
  static v8::Local<v8::Object> New(WayPtr way);

private:

  WayJs() = default;
  WayJs(ConstWayPtr w);
  ~WayJs() override = default;

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getNodeCount(const v8::FunctionCallbackInfo<v8::Value>& args);

  ConstWayPtr _constWay;
  WayPtr _way;
  static v8::Persistent<v8::Function> _constructor;

  void _setWay(ConstWayPtr w) { _constWay = w; _way.reset(); }
  void _setWay(WayPtr w) { _way = w; _constWay = w; }
};

}

#endif // WAYJS_H
