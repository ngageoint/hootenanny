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
#include "IdGeneratorJs.h"

// boost
#include <boost/any.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

namespace hoot
{

using namespace boost;

HOOT_JS_REGISTER(IdGeneratorJs)

IdGeneratorJs::IdGeneratorJs(IdGeneratorPtr idGen)
{
  _idGen = idGen;
}

IdGeneratorJs::~IdGeneratorJs()
{
}

void IdGeneratorJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(IdGenerator::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QByteArray utf8 = QString::fromStdString(opNames[i]).replace("hoot::", "").toUtf8();
    const char* n = utf8.data();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol(opNames[i].data()));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  String::New(IdGenerator::className().data()));

    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol(n), constructor);
  }
}

Handle<Value> IdGeneratorJs::New(const Arguments& args) {
  HandleScope scope;

  QString className = str(args.This()->GetConstructorName());
  if (className == "Object")
  {
    return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
      "Invalid IdGenerator. Did you forget 'new'?")));
  }
  IdGenerator* idGen = Factory::getInstance().constructObject<IdGenerator>(className);
  IdGeneratorJs* obj = new IdGeneratorJs(IdGeneratorPtr(idGen));
  obj->Wrap(args.This());

  PopulateConsumersJs::populateConsumers<IdGenerator>(idGen, args);

  return args.This();
}

}

