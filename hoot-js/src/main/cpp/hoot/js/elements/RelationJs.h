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
#ifndef RELATIONJS_H
#define RELATIONJS_H

// hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/js/elements/ElementJs.h>

// node.js
#include <hoot/js/SystemNodeJs.h>

// Qt
#include <QString>

// Standard
#include <memory>

namespace hoot
{

class OsmMapOperation;

class RelationJs : public ElementJs
{
public:
  static void Init(v8::Handle<v8::Object> target);

  virtual ConstElementPtr getConstElement() const { return getConstRelation(); }
  ConstRelationPtr getConstRelation() const { return _constRelation; }
  virtual ElementPtr getElement() { return getRelation(); }
  RelationPtr getRelation() { assert(_relation); return _relation; }

  static v8::Handle<v8::Object> New(ConstRelationPtr relation);
  static v8::Handle<v8::Object> New(RelationPtr relation);

private:
  RelationJs(ConstRelationPtr w);
  RelationJs();
  ~RelationJs();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void replaceElement(const v8::FunctionCallbackInfo<v8::Value>& args);

  QString _className;
  ConstRelationPtr _constRelation;
  RelationPtr _relation;
  static v8::Persistent<v8::Function> _constructor;

  void _setRelation(ConstRelationPtr r) { _constRelation = r; _relation.reset(); }
  void _setRelation(RelationPtr r) { _relation = r; _constRelation = r; }
};

}

#endif // RELATIONJS_H
