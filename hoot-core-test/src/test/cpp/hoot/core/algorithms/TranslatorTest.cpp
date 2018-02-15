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
 * @copyright Copyright (C) 2013, 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/Soundex.h>
#include <hoot/core/algorithms/Translator.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Qt
#include <QString>
#include <QStringList>

// Standard
#include <string>

namespace hoot
{

class TranslatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TranslatorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    QStringList result;
    result << "building";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll("gedung"));

    result.clear();
    result << "office building";
    result << "agency building";
    result << "bureau building";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll("kantor gedung"));

    result.clear();
    result << "hospital";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll("rumah sakit"));

    result.clear();
    result << "general hospital office";
    result << "general hospital agency";
    result << "general hospital bureau";
    result << "hospital office";
    result << "hospital agency";
    result << "hospital bureau";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll("rumah sakit umum kantor"));

    result.clear();
    result << "ryba office bar";
    result << "ryba agency bar";
    result << "ryba bureau bar";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll(
      QString::fromUtf8("рыба kantor bar")));

    result.clear();
    result << "embassy hungaria";
    CPPUNIT_ASSERT_EQUAL(result, Translator::getInstance().toEnglishAll(
      QString::fromUtf8("Kedutaan Besar Hungaria")));
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TranslatorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TranslatorTest, "quick");

}
