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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

// Standard Includes
#include <iostream>
using namespace std;

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <tgs/StreamUtils.h>
#include <tgs/Xml/XmlHelper.h>

#include "../PluginFactory.h"

using namespace Tgs;
namespace Tgs
{
  class XmlHelperTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(XmlHelperTest);
    CPPUNIT_TEST(testSplit);
    CPPUNIT_TEST(testTrim);
    CPPUNIT_TEST(testParseAttributes);
    CPPUNIT_TEST(testParseTagValue);
    CPPUNIT_TEST_SUITE_END();
  public:

    void check(string s, vector<string> v)
    {
      stringstream strm;
      strm << v;
      CPPUNIT_ASSERT_EQUAL(s, strm.str());
    }

    void check(string s, HashMap<string, string> v)
    {
      stringstream strm;
      strm << v;
      CPPUNIT_ASSERT_EQUAL(s, strm.str());
    }

    void testSplit()
    {
      check("[3]{a, bbb, ccccc}", XmlHelper::split("a bbb ccccc", " "));
      check("[3]{, a, bbb  ccccc}", XmlHelper::split(",a,bbb  ccccc", ","));
      check("[5]{a, bbb  , , ccccc, }", XmlHelper::split("a,bbb  ,,ccccc,", ","));
    }

    void testTrim()
    {
      CPPUNIT_ASSERT_EQUAL(string("boo hoo"), XmlHelper::trim("   boo hoo  "));
      CPPUNIT_ASSERT_EQUAL(string("boo hoo"), XmlHelper::trim("\t   boo hoo  \n"));
      CPPUNIT_ASSERT_EQUAL(string("boo \thoo"), XmlHelper::trim("boo \thoo"));
      CPPUNIT_ASSERT_EQUAL(string(""), XmlHelper::trim("  \t \n"));
    }

    void testParseAttributes()
    {
      check("[2]{(a, 2), (bar, 1)}", XmlHelper::parseAttributes("<foo bar=1 a=2  >"));
      check("[2]{(a, 2), (bar, 1)}", XmlHelper::parseAttributes(" <foo bar=1   a=2   >  \n"));
      check("[2]{(a, foo), (bar, my text)}", XmlHelper::parseAttributes(" <foo bar='my text'  a='foo' >  \n"));
    }

    void testParseTagValue()
    {
      string tag, value;
      XmlHelper::parseTagValue("<Foo>MyText</Foo>", tag, value);
      CPPUNIT_ASSERT_EQUAL(string("Foo"), tag);
      CPPUNIT_ASSERT_EQUAL(string("MyText"), value);

      XmlHelper::parseTagValue(" <BlahBlah>This is a big sentance. 19.2</BlahBlah> ", tag, value);
      CPPUNIT_ASSERT_EQUAL(string("BlahBlah"), tag);
      CPPUNIT_ASSERT_EQUAL(string("This is a big sentance. 19.2"), value);
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::XmlHelperTest, PluginFactory::testName());
