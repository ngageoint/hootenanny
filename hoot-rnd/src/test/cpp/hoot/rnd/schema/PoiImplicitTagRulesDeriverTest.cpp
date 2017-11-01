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
#include <hoot/rnd/io/ImplicitTagRulesSqliteReader.h>
#include <hoot/rnd/util/FixedLengthString.h>

// Tgs
#include <tgs/BigContainers/BigMap.h>
#include <tgs/System/Time.h>

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
//  CPPUNIT_TEST(qtMapLongKeyTest);
//  CPPUNIT_TEST(qtMapStringKeyTest);
//  CPPUNIT_TEST(rawStxxlLongKeyTest);
//  CPPUNIT_TEST(rawStxxlStringKeyTest);
//  CPPUNIT_TEST(bigMapLongKeyTest);
//  CPPUNIT_TEST(bigMapStringKeyTest);
  //TODO
  //CPPUNIT_TEST(runInputTranslationScriptSizeMismatchTest);
  //CPPUNIT_TEST(runEqualsInNameTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString inDir() { return "test-files/io/PoiImplicitTagRulesDeriverTest"; }
  static QString outDir() { return "test-output/io/PoiImplicitTagRulesDeriverTest"; }

  //static const long NUM_ITERATIONS = 100000;

//  //.02s
//  void qtMapLongKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
//    QMap<long, long> uut;

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      uut[i] = i;
//      if (i % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      const long val = uut[i];
//      if (val % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//     LOG_INFO("qtMapLongKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

//  //.13s
//  void qtMapStringKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
//    QMap<QString, long> uut;

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      uut[QString::number(i)] = i;
//      if (i % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      const long val = uut[QString::number(i)];
//      if (val % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    LOG_INFO("qtMapStringKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

//  //1.03s
//  void rawStxxlLongKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
//    LongToLongMap uut(
//      FixedLengthStringToLongMap::node_block_type::raw_size * 10,
//      FixedLengthStringToLongMap::node_block_type::raw_size * 10);

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      uut[i] = i;
//      if (i % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }
//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      long val = uut[i];
//      if (val % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    LOG_INFO("rawStxxlLongKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

//  //17.65s
//  void rawStxxlStringKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
//    FixedLengthStringToLongMap uut(
//      FixedLengthStringToLongMap::node_block_type::raw_size * 10,
//      FixedLengthStringToLongMap::node_block_type::raw_size * 10);

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      uut[PoiImplicitTagRulesDeriver::qStrToFixedLengthStr(QString::number(i))] = i;
//      if (i % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }
//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      long val = uut[PoiImplicitTagRulesDeriver::qStrToFixedLengthStr(QString::number(i))];
//      if (val % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    LOG_INFO("rawStxxlStringKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

//  //.035s
//  void bigMapLongKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
//    Tgs::BigMap<long, long> uut;

//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      uut[i] = i;
//      if (i % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }
//    for (long i = 0; i < NUM_ITERATIONS; i++)
//    {
//      long val = uut[i];
//      if (val % (NUM_ITERATIONS / 10) == 0)
//      {
//        LOG_VART(i);
//      }
//    }

//    LOG_INFO("bigMapLongKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

//  //
//  void bigMapStringKeyTest()
//  {
//    const double start = Tgs::Time::getTime();
////    Tgs::BigMap<FixedLengthString, long> uut;

////    for (long i = 0; i < NUM_ITERATIONS; i++)
////    {
////      uut[PoiImplicitTagRulesDeriver::qStrToFixedLengthStr(QString::number(i))] = i;
////      if (i % (NUM_ITERATIONS / 10) == 0)
////      {
////        LOG_VART(i);
////      }
////    }
////    for (long i = 0; i < NUM_ITERATIONS; i++)
////    {
////      long val = uut[PoiImplicitTagRulesDeriver::qStrToFixedLengthStr(QString::number(i))];
////      if (val % (NUM_ITERATIONS / 10) == 0)
////      {
////        LOG_VART(i);
////      }
////    }

//    LOG_INFO("bigMapStringKeyTest took " << Tgs::Time::getTime() - start << " seconds.");
//  }

  void runBasicTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest-out.sqlite";

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver().deriveRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(inDir() + "/PoiImplicitTagRulesDeriverTest-runBasicTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(438L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runTypeKeysTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runTypeKeysTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runTypeKeysTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    QStringList typeKeys;
    typeKeys.append("amenity");
    typeKeys.append("tourism");
    typeKeys.append("building");

    PoiImplicitTagRulesDeriver().deriveRules(inputs, translationScripts, outputs, typeKeys);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runTypeKeysTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(247L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMinOccuranceThresholdTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinOccuranceThresholdTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMinOccuranceThresholdTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver().deriveRules(inputs, translationScripts, outputs, QStringList(), 4);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runMinOccuranceThresholdTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(109L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runMultipleInputsTest()
  {
    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/yemen-crop-2.osm.pbf");
    inputs.append(inDir() + "/philippines-1.osm.pbf");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMultipleInputsTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runMultipleInputsTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver().deriveRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runMultipleInputsTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(599L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }

  void runNameCaseTest()
  {
    //Case is actually already handled correctly in runBasicTest, but this smaller input dataset
    //will make debugging case problems easier, if needed.

    QDir().mkpath(outDir());

    QStringList inputs;
    inputs.append(inDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest.osm");

    QStringList translationScripts;
    translationScripts.append("translations/OSM_Ingest.js");

    const QString jsonOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest-out.json";
    const QString dbOutputFile =
      outDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest-out.sqlite";
    QStringList outputs;
    outputs.append(jsonOutputFile);
    outputs.append(dbOutputFile);

    PoiImplicitTagRulesDeriver().deriveRules(inputs, translationScripts, outputs);

    HOOT_FILE_EQUALS(
      inDir() + "/PoiImplicitTagRulesDeriverTest-runNameCaseTest.json", jsonOutputFile);

    ImplicitTagRulesSqliteReader dbReader;
    dbReader.open(dbOutputFile);
    CPPUNIT_ASSERT_EQUAL(9L, dbReader.getRuleWordPartCount());
    dbReader.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiImplicitTagRulesDeriverTest, "quick");

}
