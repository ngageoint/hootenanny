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
 * @copyright Copyright (C) 2013, 2014, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class StringUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(StringUtilsTest);
  CPPUNIT_TEST(runHasAlphabeticCharTest);
  CPPUNIT_TEST(jsonParseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runHasAlphabeticCharTest()
  {
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("test"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("234"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("(*&%<?"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("Şiḩḩī"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("_____"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("-かまくらふしきあん"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter(" か しきあん"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter(""));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("  "));
  }

  void jsonParseTest()
  {
    const QString jsonInput =
      "{\"apps\":[{\"name\":\"TikaLanguageDetector\",\"description\":\"The language detection portion of a library which detects and extracts metadata and text from many different file types\",\"url\":\"https://tika.apache.org\"},{\"name\":\"OpenNlpLanguageDetector\",\"description\":\"The language detector portion of a machine learning based toolkit for the processing of natural language text\",\"url\":\"https://opennlp.apache.org\"}]}";
    boost::shared_ptr<boost::property_tree::ptree> propTree =
      StringUtils::jsonStringToPropTree(jsonInput);
    std::stringstream outputStrStream;
    boost::property_tree::json_parser::write_json(outputStrStream, *propTree);
    HOOT_STR_EQUALS(jsonInput, outputStrStream.str());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringUtilsTest, "quick");

}

