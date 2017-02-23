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
#ifndef OSMMAPOPERATIONJS_H
#define OSMMAPOPERATIONJS_H

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

class OsmMapOperationJs : public node::ObjectWrap
{
public:
  static void Init(v8::Handle<v8::Object> target);

  OsmMapOperation* getMapOp() { return _op.get(); }

private:
  OsmMapOperationJs(OsmMapOperation *op);
  ~OsmMapOperationJs();

  static v8::Handle<v8::Value> apply(const v8::Arguments& args);
  static v8::Handle<v8::Value> applyAndGetResult(const v8::Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

  QString _className;
  auto_ptr<OsmMapOperation> _op;
};

}

#endif // OSMMAPOPJS_H
