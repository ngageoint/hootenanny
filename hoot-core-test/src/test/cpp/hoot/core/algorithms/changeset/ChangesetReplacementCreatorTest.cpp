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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator1.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/util/GeometryUtils.h>

namespace hoot
{

/*
 * Currently, this primarily tests some input validation checks, as it has been easier to test the
 * actual changeset generation from command line tests instead
 * (ServiceChangesetReplacement*Test.sh), since it is easy to see the results of applying the
 * changesets back to the source data. That being said, the command line tests are slow and
 * numerous. Eventually, when C&R is more stable the command line tests will probably be rolled back
 * to just a handful and many of them will be converted to unit tests with a workflow similar to,
 * but not as complicated as, that in ServiceChangesetReplacementGridTest.
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  CPPUNIT_TEST(runInvalidGeometryFilterTest);
  CPPUNIT_TEST(runInvalidReplacementFilterTest);
  CPPUNIT_TEST(runInvalidRetainmentFilterTest);
  CPPUNIT_TEST(runNonBoundableReaderTest);
  CPPUNIT_TEST(runGeoJsonTest);
  CPPUNIT_TEST(runInvalidFilterConfigOptsTest);
  CPPUNIT_TEST(runConvertOpsTest);
  CPPUNIT_TEST(runFullReplacmentWithRetainmentFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest()
  {
  }

  void runInvalidGeometryFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;
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

  void runInvalidReplacementFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;

    try
    {
      changesetCreator.setReplacementFilters(QStringList("hoot::AddAttributesVisitor"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional input filter"));

    try
    {
      changesetCreator.setReplacementFilters(QStringList("hoot::PoiCriterion"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional input filter"));
  }

  void runInvalidRetainmentFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;

    try
    {
      changesetCreator.setRetainmentFilters(QStringList("hoot::AddAttributesVisitor"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional input filter"));

    try
    {
      changesetCreator.setRetainmentFilters(QStringList("hoot::PoiCriterion"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid additional input filter"));
  }

  void runNonBoundableReaderTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;
    try
    {
      changesetCreator.create(
        "test-files/cmd/quick/ConvertGeoNames.geonames", "test2.osm", geos::geom::Envelope(),
        "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(
      exceptionMsg.endsWith("must implement Boundable for replacement changeset derivation."));
  }

  void runGeoJsonTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;
    try
    {
      changesetCreator.create("test1.geojson", "test2.osm", geos::geom::Envelope(), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("GeoJSON inputs are not supported by replacement changeset derivation.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runInvalidFilterConfigOptsTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;

    // the filter can be any non-geometry crit here
    changesetCreator.setReplacementFilters(
      QStringList(QString::fromStdString(TagCriterion::className())));
    try
    {
      changesetCreator.setReplacementFilterOptions(QStringList("blah"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid filter configuration option"));

    // the filter can be any non-geometry crit here
    changesetCreator.setRetainmentFilters(
      QStringList(QString::fromStdString(TagCriterion::className())));
    try
    {
      changesetCreator.setRetainmentFilterOptions(QStringList("blah"));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.startsWith("Invalid filter configuration option"));
  }

  void runConvertOpsTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;
    // the convert ops added here can contain any op
    conf().set(
      ConfigOptions::getConvertOpsKey(),
      QStringList(QString::fromStdString(DuplicateNodeRemover::className())));
    try
    {
      changesetCreator.create("test1.osm", "test2.osm", geos::geom::Envelope(), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Replacement changeset derivation does not support convert operations.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runFullReplacmentWithRetainmentFilterTest()
  {
    QString exceptionMsg;
    ChangesetReplacementCreator1 changesetCreator;
    changesetCreator.setFullReplacement(true);
    // the filter can be any non-geometry crit here
    changesetCreator.setRetainmentFilters(
      QStringList(QString::fromStdString(TagCriterion::className())));
    try
    {
      changesetCreator.create("test1.osm", "test2.osm", geos::geom::Envelope(), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Both full reference data replacement and a reference data retainment filter may not "
              "be specified for replacement changeset derivation.").toStdString(),
      exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "quick");

}
