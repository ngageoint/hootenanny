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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  // We're already testing API DB inputs with command tests.
  CPPUNIT_TEST(runPolyLenientOsmTest);
  CPPUNIT_TEST(runPolyStrictOsmTest);
  CPPUNIT_TEST(runPoiStrictOsmTest);
  CPPUNIT_TEST(runLinearLenientOsmTest);
  CPPUNIT_TEST(runLinearStrictOsmTest);
  CPPUNIT_TEST(runPolyLenientJsonTest);
  CPPUNIT_TEST(runPolyStrictJsonTest);
  CPPUNIT_TEST(runPoiStrictJsonTest);
  CPPUNIT_TEST(runLinearLenientJsonTest);
  CPPUNIT_TEST(runLinearStrictJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/algorithms/changeset/ChangesetReplacementCreatorTest/")
  {
  }

  void runPolyLenientOsmTest()
  { 

  }

  void runPolyStrictOsmTest()
  {

  }

  void runPoiStrictOsmTest()
  {

  }

  void runLinearLenientOsmTest()
  {

  }

  void runLinearStrictOsmTest()
  {

  }

  void runPolyLenientJsonTest()
  {

  }

  void runPolyStrictJsonTest()
  {

  }

  void runPoiStrictJsonTest()
  {

  }

  void runLinearLenientJsonTest()
  {

  }

  void runLinearStrictJsonTest()
  {

  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "slow");

}
