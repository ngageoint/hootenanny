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
  CPPUNIT_TEST(runTypeKeysTest);
  CPPUNIT_TEST(runMinOccuranceThresholdTest);
  CPPUNIT_TEST(runMultipleInputsTest);
  CPPUNIT_TEST(runNameCaseTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/io/PoiImplicitTagRulesDeriverTest"; }
  static QString outDir() { return "test-output/io/PoiImplicitTagRulesDeriverTest"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");

    const ImplicitTagRules rules = PoiImplicitTagRulesDeriver().deriveRules(inputs);

    const QString outputFile = outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.json";
    writeOutputFile(rules, outputFile);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest.json", outputFile);
  }

  void runTypeKeysTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");

    QStringList typeKeys;
    typeKeys.append("amenity");
    typeKeys.append("tourism");
    typeKeys.append("building");

    const ImplicitTagRules rules = PoiImplicitTagRulesDeriver().deriveRules(inputs, typeKeys);

    const QString outputFile = outDir() + "/PoiImplicitTagRulesDeriverTest-runTypeKeysTest-out.json";
    writeOutputFile(rules, outputFile);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRulesDeriverTest-runTypeKeysTest.json", outputFile);
  }

  void runMinOccuranceThresholdTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");

    const ImplicitTagRules rules =
      PoiImplicitTagRulesDeriver().deriveRules(inputs, QStringList(), 4);

    const QString outputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinOccuranceThresholdTest-out.json";
    writeOutputFile(rules, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runMinOccuranceThresholdTest.json", outputFile);
  }

  void runMultipleInputsTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    inputs.append(inDir() + "/philippines-1.osm.pbf");

    const ImplicitTagRules rules = PoiImplicitTagRulesDeriver().deriveRules(inputs);

    const QString outputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMultipleInputsTest-out.json";
    writeOutputFile(rules, outputFile);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runMultipleInputsTest.json", outputFile);
  }

  void runNameCaseTest()
  {
    //Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    //will make debugging case problems easier, if needed.

    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest.osm");

    const ImplicitTagRules rules = PoiImplicitTagRulesDeriver().deriveRules(inputs);

    const QString outputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest-out.json";
    writeOutputFile(rules, outputFile);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest.json", outputFile);
  }

  void writeOutputFile(const ImplicitTagRules& rules, const QString outputFile)
  {
    ImplicitTagRulesJsonWriter writer;
    writer.open(outputFile);
    writer.write(rules);
    writer.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRulesDeriverTest, "quick");

}
