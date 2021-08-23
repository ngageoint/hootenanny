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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/js/HootJsStable.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/PluginContext.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>

// Qt
#include <QVariant>


#include <string>

using namespace v8;

namespace hoot
{

class DataConvertJsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(DataConvertJsTest);
  CPPUNIT_TEST(qvariantTest);
  CPPUNIT_TEST_SUITE_END();

public:

  // what can I say? I'm lazy and I don't like extra verbose mark ups.
  QString fixQ(QString s)
  {
    return s.replace("'", "\"");
  }

  void qvariantTest()
  {
    Isolate* current = v8::Isolate::GetCurrent();
    HandleScope handleScope(current);

    // Create a new context.
    Local<Context> context = Context::New(current);

    // Enter the created context for compiling and
    // running the hello world script.
    Context::Scope context_scope(context);

    HOOT_STR_EQUALS(fixQ("'hello world'"), toJson(toV8(QVariant("hello world"))));
    HOOT_STR_EQUALS(fixQ("1"), toJson(toV8(QVariant(1))));
    HOOT_STR_EQUALS(fixQ("3.14"), toJson(toV8(QVariant(3.14))));
    HOOT_STR_EQUALS(fixQ("true"), toJson(toV8(QVariant(true))));
    HOOT_STR_EQUALS(fixQ("['Foo',1,3.14]"), toJson(toV8(QVariantList() << "Foo" << 1 << 3.14)));

    HOOT_STR_EQUALS(fixQ("1"), toCpp<QVariant>(fromJson("1")));
    HOOT_STR_EQUALS(fixQ("foo"), toCpp<QVariant>(fromJson(fixQ("'foo'"))));

    QVariant v = toCpp<QVariant>(fromJson(fixQ("[1,2]")));
    HOOT_STR_EQUALS(QVariant::List, v.type());

    // TODO: finish QVariantList tests

    QVariantMap m;
    m.insert("one",fixQ("[3,4,5]"));
    m.insert("two",2);

    HOOT_STR_EQUALS(fixQ("{'one':'[3,4,5]','two':2}"), toJson(toV8(m)));

    // strings should allow converting numbers into strings
    HOOT_STR_EQUALS("1", toCpp<QString>(fromJson(fixQ("1"))));
    // strings shouldn't automagically convert complex types
    CPPUNIT_ASSERT_THROW(toCpp<QString>(fromJson(fixQ("[1,2,3]"))), IllegalArgumentException);

    HOOT_STR_EQUALS("[3]{(1, 2), (a, b), (foo, 3)}",
      toCpp<QVariantMap>(fromJson(fixQ("{'1':2,'a':'b','foo':3}"))));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConvertJsTest, "quick");

}
