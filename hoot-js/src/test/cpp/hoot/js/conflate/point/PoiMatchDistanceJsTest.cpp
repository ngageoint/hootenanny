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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/js/HootJsStable.h>
#include <hoot/js/conflate/point/PoiMatchDistanceJs.h>
#include <hoot/js/io/DataConvertJs.h>

using namespace v8;

namespace hoot
{

class ScriptMatchTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ScriptMatchTest);
  CPPUNIT_TEST(runConfigOptionTest);
  CPPUNIT_TEST(runFileTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ScriptMatchTest() :
  HootTestFixture("test-files/conflate/point/PoiMatchDistanceTest/", UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  void runConfigOptionTest()
  {
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    // Create a new context.
    Local<Context> context = Context::New(current);
    // Enter the created context for compiling and
    // running the hello world script.
    Context::Scope context_scope(context);

//    FunctionCallbackInfo<Value> args;
//    PoiMatchDistanceJs::getMatchDistances(args);
//    Handle<Array> arr = Handle<Array>::Cast(args[0]);
//    QVariantList outArgs;
//    toCpp(arr, outArgs);
//    LOG_VARW(outArgs.size());
    // TODO: finish
  }

  void runFileTest()
  {
    // TODO: finish
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ScriptMatchTest, "quick");

}
