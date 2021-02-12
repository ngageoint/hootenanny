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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#include "v8Engine.h"

using namespace std;
using namespace v8;

namespace hoot
{

bool v8Engine::_needPlatform = false;

v8Engine::v8Engine()
{
  //  Setup and initialize the platform
  if (v8Engine::_needPlatform)
  {
    V8::InitializeICUDefaultLocation(NULL);
    V8::InitializeExternalStartupData(NULL);
    _platform.reset(platform::CreateDefaultPlatform());
    V8::InitializePlatform(_platform.get());
    //  Initialize v8
    V8::Initialize();
    //  Create the main isolate
    _allocator.reset(ArrayBuffer::Allocator::NewDefaultAllocator());
    Isolate::CreateParams params;
    params.array_buffer_allocator = _allocator.get();
    _isolate = Isolate::New(params);
    _isolateScope.reset(new Isolate::Scope(_isolate));
    //  Create the main context
    _locker.reset(new Locker(_isolate));
    HandleScope handleScope(_isolate);
    _context.reset(new Persistent<Context>(_isolate, Context::New(_isolate)));
    Local<Context> context = ToLocal(_context.get());
    _scopeContext.reset(new Context::Scope(context));
  }
  else
  {
    _isolate = v8::Isolate::GetCurrent();
  }
}

v8Engine::~v8Engine()
{
  if (v8Engine::_needPlatform)
  {
    _scopeContext.reset();
    _locker.reset();
    _isolateScope.reset();
    //  Dispose of the v8 subsystem
    _isolate->Dispose();
    V8::Dispose();
    //  Shutdown the platform
    V8::ShutdownPlatform();
  }
}

v8Engine& v8Engine::getInstance()
{
  //  Local static singleton instance
  static v8Engine instance;
  return instance;
}

Isolate* v8Engine::getIsolate()
{
  return getInstance()._isolate;
}

void v8Engine::setPlatformInit(bool needsPlatform)
{
  _needPlatform = needsPlatform;
}


}
