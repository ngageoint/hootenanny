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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/UuidHelperJs.h>

// Qt
#include <QVariant>

#include <iostream>
#include <string>

using namespace v8;

namespace hoot
{

class UuidHelperJsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(UuidHelperJsTest);
  CPPUNIT_TEST(uuidHashTest);
  CPPUNIT_TEST_SUITE_END();

public:

  // Jasons Code: "what can I say? I'm lazy and I don't like extra verbose mark ups."
  // Heck yeah, this is my code! Are you saying you like crazy verbose and difficult to read tests!?
  // -JRS
  QString fixQ(QString s)
  {
    return s.replace("'", "\"");
  }

  void uuidHashTest()
  {
    std::shared_ptr<PluginContext> _pc(new PluginContext());
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);
    Context::Scope context_scope(_pc->getContext(current));

    // From the standard Uuid Test
    // QUuid r = UuidHelper::createUuid5("foo", QUuid("{6ba7b812-9dad-11d1-80b4-00c04fd430c8}"));
    // HOOT_STR_EQUALS("{bca95adb-b5f1-564f-96a7-6355c52d1fa7}", r.toString());

    conf().set(ConfigOptions().getUuidHelperRepeatableKey(), true);
    HOOT_STR_EQUALS(fixQ("'{b18057ff-736d-5d20-b873-837f0c172e33}'"),
                    _pc->eval("hoot.UuidHelper.createUuid()"));

    HOOT_STR_EQUALS(fixQ("'{f1a68c36-8474-5aa5-93ab-8a95ef41050b}'"),
                    _pc->eval("hoot.UuidHelper.createUuid5('bar')"));

    HOOT_STR_EQUALS(fixQ("'{bca95adb-b5f1-564f-96a7-6355c52d1fa7}'"),
                    _pc->eval("hoot.UuidHelper.createUuid5('foo', '{6ba7b812-9dad-11d1-80b4-00c04fd430c8}')"));

  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UuidHelperJsTest, "quick");

}

