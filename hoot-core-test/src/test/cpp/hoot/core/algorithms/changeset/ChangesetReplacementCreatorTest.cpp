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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

/*
 * This only tests some invalidation checking, as its easier to test the changeset generation in
 * command line tests (ChangesetReplacement*Test.sh).
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  CPPUNIT_TEST(runInvalidGeometryFilterTest);
  CPPUNIT_TEST(runInvalidAdditionalFilterTest);
  CPPUNIT_TEST(runNonBoundableReaderTest);
  CPPUNIT_TEST(runGeoJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/ralgorithms/changeset/ChangesetReplacementCreatorTest/")
  {
  }

  void runInvalidGeometryFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator changesetCreator;
    try
    {
      changesetCreator.setGeometryFilters(QStringList("hoot::TagCriterion"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid feature geometry type filter"));
  }

  void runInvalidAdditionalFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator changesetCreator;

    try
    {
      changesetCreator.setAdditionalFilters(QStringList("hoot::AddAttributesVisitor"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional filter"));

    try
    {
      changesetCreator.setAdditionalFilters(QStringList("hoot::PoiCriterion"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional filter"));
  }

  void runNonBoundableReaderTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator changesetCreator;
    try
    {
      changesetCreator.create(
        "test-files/cmd/quick/ConvertGeoNames.geonames", "test2.osm", geos::geom::Envelope(), "");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.endsWith("must implement Boundable."));
  }

  void runGeoJsonTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator changesetCreator;
    try
    {
      changesetCreator.create("test1.geojson", "test2.osm", geos::geom::Envelope(), "");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("GeoJSON inputs are not supported.").toStdString(), exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "quick");

}
