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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetCutOnlyCreator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/geometry/GeometryUtils.h>

namespace hoot
{

/*
 * Currently, this primarily tests some input validation checks, as it has been easier to test the
 * actual changeset generation from command line tests instead
 * (ServiceChangesetReplacement*Test.sh), since it is easy to see the results of applying the
 * changesets back to the source data. That being said, the command line tests are slow and
 * numerous. Eventually when C&R is more stable, the command line tests will probably be rolled back
 * to just a handful and many of them will be converted to unit tests with a workflow similar to
 * that in ServiceChangesetReplacementGridTest.
 *
 * UPDATE 10/22/20: These tests have to be run against ChangesetCutOnlyCreator instead of
 * ChangesetReplacementCreator for now since ChangesetReplacementCreator doesn't have a working
 * additional filter implementation (#4267).
 */
class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  CPPUNIT_TEST(runNonBoundableReaderTest);
  CPPUNIT_TEST(runGeoJsonTest);
  CPPUNIT_TEST(runConvertOpsTest);
  CPPUNIT_TEST(runInvalidBoundsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest()
  {
  }

  void runNonBoundableReaderTest()
  {
    QString exceptionMsg;
    ChangesetCutOnlyCreator changesetCreator;
    try
    {
      changesetCreator.create(
        "test-files/cmd/quick/ConvertGeoNames.geonames", "test2.osm",
        geos::geom::Envelope(0, 0, 0, 0), "out.osm");
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
    ChangesetCutOnlyCreator changesetCreator;
    try
    {
      changesetCreator.create(
        "test1.geojson", "test2.osm", geos::geom::Envelope(0, 0, 0, 0), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("GeoJSON inputs are not supported by replacement changeset derivation.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runConvertOpsTest()
  {
    QString exceptionMsg;
    ChangesetCutOnlyCreator changesetCreator;
    // the convert ops added here can contain any op
    conf().set(
      ConfigOptions::getConvertOpsKey(), QStringList(DuplicateNodeRemover::className()));
    try
    {
      changesetCreator.create(
        "test1.osm", "test2.osm", geos::geom::Envelope(0, 0, 0, 0), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Replacement changeset derivation does not support convert operations.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runInvalidBoundsTest()
  {
    // This is just doing null checks. GeometryUtilsTest goes into more detail on validation of
    // these bounds from string.

    QString exceptionMsg;
    ChangesetCutOnlyCreator changesetCreator;

    try
    {
      changesetCreator.create(
        "test1.osm", "test2.osm", std::shared_ptr<geos::geom::Polygon>(), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid replacement bounds passed to changeset replacement derivation.").toStdString(),
      exceptionMsg.toStdString());

    try
    {
      changesetCreator.create(
        "test1.osm", "test2.osm", geos::geom::Envelope(), "out.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Invalid replacement bounds passed to changeset replacement derivation.").toStdString(),
      exceptionMsg.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "quick");

}
