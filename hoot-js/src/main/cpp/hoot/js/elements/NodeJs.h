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
#ifndef NODEJS_H
#define NODEJS_H

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/js/elements/ElementJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

namespace hoot
{
using namespace std;
using namespace v8;

class OsmMapOperation;

class NodeJs : public ElementJs
{
public:
  static void Init(v8::Handle<v8::Object> target);

  virtual ConstElementPtr getConstElement() const { return getConstNode(); }
  ConstNodePtr getConstNode() const { return _constNode; }
  virtual ElementPtr getElement() { return getNode(); }
  NodePtr getNode() { assert(_node); return _node; }

  static Handle<Object> New(ConstNodePtr n);
  static Handle<Object> New(NodePtr n);

private:
  NodeJs(ConstNodePtr n);
  NodeJs();
  ~NodeJs();

  static v8::Handle<v8::Value> getX(const v8::Arguments& args);
  static v8::Handle<v8::Value> getY(const v8::Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  QString _className;
  ConstNodePtr _constNode;
  NodePtr _node;
  static Persistent<Function> _constructor;

  void _setNode(ConstNodePtr n) { _constNode = n; _node.reset(); }
  void _setNode(NodePtr n) { _constNode = n; _node = n; }
};

}

#endif // NODEJS_H
