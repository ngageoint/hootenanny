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
#ifndef NODEJS_H
#define NODEJS_H

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/js/elements/ElementJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt


namespace hoot
{

class NodeJs : public ElementJs
{
public:

  static void Init(v8::Local<v8::Object> target);

  ConstElementPtr getConstElement() const override { return getConstNode(); }
  ConstNodePtr getConstNode() const { return _constNode; }
  ElementPtr getElement() override { return getNode(); }
  NodePtr getNode() const { assert(_node); return _node; }

  static v8::Local<v8::Object> New(ConstNodePtr n);
  static v8::Local<v8::Object> New(NodePtr n);

private:

  NodeJs(ConstNodePtr n) : _constNode(n) { }
  NodeJs() = default;
  ~NodeJs() = default;

  static void getX(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void getY(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  ConstNodePtr _constNode;
  NodePtr _node;
  static v8::Persistent<v8::Function> _constructor;

  void _setNode(ConstNodePtr n) { _constNode = n; _node.reset(); }
  void _setNode(NodePtr n) { _constNode = n; _node = n; }
};

}

#endif // NODEJS_H
