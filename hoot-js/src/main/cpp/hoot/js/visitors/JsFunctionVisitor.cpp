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
#include "JsFunctionVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/v8Engine.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/DataConvertJs.h>

using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, JsFunctionVisitor)

void JsFunctionVisitor::visit(const ConstElementPtr& e)
{
  Isolate* current = v8Engine::getIsolate();
  HandleScope handleScope(current);
  Local<Context> context(current->GetCurrentContext());

  Handle<Value> jsArgs[3];

  if (_func.IsEmpty())
  {
    throw IllegalArgumentException("JsFunctionVisitor must have a valid function.");
  }

  Handle<Object> elementObj;
  if (_map)
  {
    ElementPtr nonConst = _map->getElement(e->getElementId());
    elementObj = ElementJs::New(nonConst);
  } else {
    elementObj = ElementJs::New(e);
  }

  int argc = 0;
  jsArgs[argc++] = elementObj;

  TryCatch trycatch;
  Handle<Value> funcResult = _func.Get(current)->Call(context->Global(), argc, jsArgs);

  if (funcResult.IsEmpty())
  {
    Local<Value> exception = trycatch.Exception();
    if (HootExceptionJs::isHootException(exception))
    {
      boost::shared_ptr<HootException> e = toCpp<boost::shared_ptr<HootException> >(exception);
      HootExceptionThrower::getInstance().rethrowPointer(e);
    }
    else
    {
      // Matt, if this conflicts with the newer more robust handling, use that.
      throw HootException(toJson(trycatch.Message()->Get()));
    }
  }
}

}
