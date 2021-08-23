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
 * @copyright Copyright (C) 2015, 2016, 2018, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/string/MostEnglishName.h>
#include <hoot/core/util/ConfigOptions.h>


namespace hoot
{

class MostEnglishNameTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MostEnglishNameTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    HOOT_STR_EQUALS(1, MostEnglishName::getInstance()->scoreName("Foo bar"));

    HOOT_STR_EQUALS(1,
      MostEnglishName::getInstance()->scoreName("Disney World"));
    HOOT_STR_EQUALS(0,
      MostEnglishName::getInstance()->scoreName(""));

    HOOT_STR_EQUALS(0,
      MostEnglishName::getInstance()->scoreName(QString::fromUtf8("улица Ильинка")));
    HOOT_STR_EQUALS(0,
      MostEnglishName::getInstance()->scoreName(
        QString::fromUtf8("улица Куйбышева (1935 г.) улица Ильинка (до 1917 г.)")));
    HOOT_STR_EQUALS(.75,
      MostEnglishName::getInstance()->scoreName(QString::fromUtf8("street Ilinka")));

    Tags t;
    t["name"] = "Calle Ilinka";
    t["alt_name"] = QString::fromUtf8("улица Ильинка;street Ilinka");
    HOOT_STR_EQUALS("street Ilinka", MostEnglishName::getInstance()->getMostEnglishName(t));

    CPPUNIT_ASSERT(MostEnglishName::getInstance()->isInDictionary("the"));

    QStringList words;
    words.append("the");
    words.append("street");
    CPPUNIT_ASSERT(MostEnglishName::getInstance()->areAllInDictionary(words));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MostEnglishNameTest, "quick");

}


