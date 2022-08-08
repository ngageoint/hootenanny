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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/NormalizePhoneNumbersVisitor.h>

// libphonenumber
#include <phonenumbers/phonenumberutil.h>
using namespace i18n::phonenumbers;

namespace hoot
{

class NormalizePhoneNumbersVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(NormalizePhoneNumbersVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runSetFormatTest);
  CPPUNIT_TEST(runSearchInTextTest);
  CPPUNIT_TEST(runInvalidRegionCodeTest);
  CPPUNIT_TEST(runConfigureTest);
  CPPUNIT_TEST_SUITE_END();

public:

  NormalizePhoneNumbersVisitorTest() :
  HootTestFixture(
    "test-files/visitors/NormalizePhoneNumbersVisitorTest/",
    "test-output/visitors/NormalizePhoneNumbersVisitorTest/")
  {
  }

  void runBasicTest()
  {
    const QString testName = "runBasicTest";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    NormalizePhoneNumbersVisitor uut;
    uut._phoneNumberNormalizer.setRegionCode("US");
    uut._phoneNumberNormalizer.setFormat("NATIONAL");
    uut._phoneNumberNormalizer.setSearchInText(false);
    map->visitRw(uut);

    const QString outputFile = _outputPath + testName + "Out.osm";
    OsmMapWriterFactory::write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(12, uut._phoneNumberNormalizer.getNumNormalized());
    HOOT_FILE_EQUALS(_inputPath + "gold.osm", outputFile);
  }

  void runSearchInTextTest()
  {
    // Going to fabricate data for this one, although there should be some actual phone numbers
    // in text somewhere in the poi/polygon regression test data.

    OsmMapPtr map = std::make_shared<OsmMap>();
    NodePtr node =
      std::make_shared<Node>(Status::Unknown1, 1, geos::geom::Coordinate(0.0, 0.0), 15.0);
    node->setTag("phone", "I'm a phone number hiding in text: (415) 431-2701 . Did you find me?");
    map->addNode(node);

    NormalizePhoneNumbersVisitor uut;
    uut._phoneNumberNormalizer.setRegionCode("US");
    uut._phoneNumberNormalizer.setFormat("NATIONAL");
    uut._phoneNumberNormalizer.setSearchInText(true);
    map->visitRw(uut);

    CPPUNIT_ASSERT_EQUAL(1, uut._phoneNumberNormalizer.getNumNormalized());
  }

  void runSetFormatTest()
  {
    // This test might be a better fit in a new PhoneNumberNormalizerTest.

    NormalizePhoneNumbersVisitor uut;

    uut._phoneNumberNormalizer.setFormat("E164");
    CPPUNIT_ASSERT_EQUAL(
      PhoneNumberUtil::PhoneNumberFormat::E164, uut._phoneNumberNormalizer._format);

    uut._phoneNumberNormalizer.setFormat("INTERNATIONAL");
    CPPUNIT_ASSERT_EQUAL(
      PhoneNumberUtil::PhoneNumberFormat::INTERNATIONAL, uut._phoneNumberNormalizer._format);

    uut._phoneNumberNormalizer.setFormat("NATIONAL");
    CPPUNIT_ASSERT_EQUAL(
      PhoneNumberUtil::PhoneNumberFormat::NATIONAL, uut._phoneNumberNormalizer._format);

    uut._phoneNumberNormalizer.setFormat("RFC3966");
    CPPUNIT_ASSERT_EQUAL(
      PhoneNumberUtil::PhoneNumberFormat::RFC3966, uut._phoneNumberNormalizer._format);

    uut._phoneNumberNormalizer.setFormat("rfc3966");
    CPPUNIT_ASSERT_EQUAL(
      PhoneNumberUtil::PhoneNumberFormat::RFC3966, uut._phoneNumberNormalizer._format);

    QString exceptionMsg;
    try
    {
      uut._phoneNumberNormalizer.setFormat("blah");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid phone number format: blah").toStdString(), exceptionMsg.toStdString());
  }

  void runInvalidRegionCodeTest()
  {
    // This test might be a better fit in a new PhoneNumberNormalizerTest.

    NormalizePhoneNumbersVisitor uut;

    QString exceptionMsg;
    try
    {
      uut._phoneNumberNormalizer.setRegionCode("");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Empty phone number region code.").toStdString(),
      exceptionMsg.toStdString());

    try
    {
      uut._phoneNumberNormalizer.setRegionCode("blah");
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid phone number region code: BLAH").toStdString(),
      exceptionMsg.toStdString());
  }

  void runConfigureTest()
  {
    const QString testName = "runConfigureTest";

    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(
      map,
      "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm",
      false,
      Status::Unknown1);

    Settings settings;
    settings.set(ConfigOptions::getPhoneNumberRegionCodeKey(), "US");
    settings.set(ConfigOptions::getPhoneNumberNormalizationFormatKey(), "NATIONAL");
    settings.set(ConfigOptions::getPhoneNumberSearchInTextKey(), false);
    NormalizePhoneNumbersVisitor uut;
    uut.setConfiguration(settings);
    map->visitRw(uut);

    const QString outputFile = _outputPath + testName + "Out.osm";
    OsmMapWriterFactory::write(map, outputFile);

    CPPUNIT_ASSERT_EQUAL(12, uut._phoneNumberNormalizer.getNumNormalized());
    HOOT_FILE_EQUALS(_inputPath + "gold.osm", outputFile);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NormalizePhoneNumbersVisitorTest, "quick");

}


