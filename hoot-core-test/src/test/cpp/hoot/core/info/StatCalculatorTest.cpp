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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/info/StatCalculator.h>
#include <hoot/core/visitors/AddressCountVisitor.h>
#include <hoot/core/visitors/NodesPerWayVisitor.h>

namespace hoot
{

/**
 * This tests error handling for StatCalculator only. Other functionality is covered in
 * StatCmdTest.sh.
 */
class StatCalculatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(StatCalculatorTest);
  CPPUNIT_TEST(runVisitorUnsupportedStatTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runVisitorUnsupportedStatTypeTest()
  {
    QStringList inputs("blah"); // What the inputs actually are doesn't matter here.
    QString visClassName = AddressCountVisitor::className();
    QString exceptionMsg;
    try
    {
      StatCalculator().calculateStat(inputs, visClassName, "average");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    const QString expectedErrorMsg =
      "Visitors passed to the stat command with a statistic type other than \"total\" must "
      "support the NumericStatistic interface.";
    CPPUNIT_ASSERT_EQUAL(expectedErrorMsg.toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StatCalculatorTest, "quick");

}
