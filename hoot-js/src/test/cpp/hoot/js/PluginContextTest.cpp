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

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>

#include <v8.h>
#include <iostream>
#include <string>

using namespace v8;

namespace hoot
{

class PluginContextTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PluginContextTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    boost::shared_ptr<PluginContext> _pc(new PluginContext());
    HandleScope handleScope;
    Context::Scope context_scope(_pc->getContext());

    Handle<Object> exports = _pc->loadScript("rules/PolygonBuilding.js");

    Handle<Value> result = _pc->call(exports, "isWholeGroup");
    HOOT_STR_EQUALS("true", result);

    result = _pc->call(_pc->getContext()->Global(), "testAdd",
      QList<QVariant>() << 1 << 2);
    HOOT_STR_EQUALS(3, result);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PluginContextTest, "quick");

}
