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
 * @copyright Copyright (C) 2013, 2014, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class StringUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(StringUtilsTest);
  CPPUNIT_TEST(runHasAlphabeticCharTest);
  CPPUNIT_TEST(jsonParseTest);
  // TODO
  //CPPUNIT_TEST(removeAllContainingTest);
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
    //The json string output from the prop tree will come back formatted with newlines and
    //indenting, so leaving spaces between items here to match the output string after its
    //simplified.
    const QString jsonInput =
      "{ \"apps\": [ { \"name\": \"TikaLanguageDetector\", \"description\": \"blah\", \"url\": \"https://tika.apache.org\" }, { \"name\": \"OpenNlpLanguageDetector\", \"description\": \"more blah\", \"url\": \"https://opennlp.apache.org\" } ] }";
    std::shared_ptr<boost::property_tree::ptree> propTree =
      StringUtils::jsonStringToPropTree(jsonInput);
    std::stringstream outputStrStream;
    boost::property_tree::json_parser::write_json(outputStrStream, *propTree);
    QString outputStr = QString::fromStdString(outputStrStream.str()).simplified().replace("\\/", "/").trimmed();
    HOOT_STR_EQUALS(jsonInput, outputStr);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringUtilsTest, "quick");

}

