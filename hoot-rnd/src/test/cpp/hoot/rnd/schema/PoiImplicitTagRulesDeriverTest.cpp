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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/rnd/schema/PoiImplicitTagRulesDeriver.h>
#include <hoot/rnd/io/ImplicitTagRulesJsonWriter.h>

// Qt
#include <QDir>

namespace hoot
{

class PoiImplicitTagRulesDeriverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiImplicitTagRulesDeriverTest);
  CPPUNIT_TEST(runBasicTest);
  //CPPUNIT_TEST(runMultipleInputsTest);
  //CPPUNIT_TEST(runTypeKeysTest);
  //CPPUNIT_TEST(runMinOccuranceThresholdTest);
  //CPPUNIT_TEST(runWrongWayTagTest);
  //CPPUNIT_TEST(runHandleSemicolonInNameTest);
  //CPPUNIT_TEST(runNameCaseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    PoiImplicitTagRulesDeriver deriver;

    QStringList inputs;
    inputs.append("/home/vagrant/hoot/tmp/yemen-crop-2.osm.pbf");

    const QMap<QString, QMap<QString, long> > rules = deriver.deriveRules(inputs);

    ImplicitTagRulesJsonWriter writer;
    writer.open("/home/vagrant/hoot/tmp/yemen-test-out.json");
    writer.write(rules);
    writer.close();
  }

  void runMultipleInputsTest()
  {

  }

  void runTypeKeysTest()
  {

  }

  void runMinOccuranceThresholdTest()
  {

  }

  void runWrongWayTagTest()
  {

  }

  void runHandleSemicolonInNameTest()
  {

  }

  void runNameCaseTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRulesDeriverTest, "quick");

}
