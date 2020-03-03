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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetCreator.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class ChangesetCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetCreatorTest);
  CPPUNIT_TEST(runUpdateVersionTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetCreatorTest() :
  HootTestFixture("test-files/algorithms/changeset/ChangesetCreatorTest/",
                  "test-output/algorithms/changeset/ChangesetCreatorTest/")
  {
  }

  void runUpdateVersionTest()
  {
    QString reference = _inputPath + "ChangesetDeriveUpdateVersion_ref.osm";
    QString secondary = _inputPath + "ChangesetDeriveUpdateVersion_sec.osm";
    QString output = _outputPath + "ChangsetDeriveUpdateVersion_output.osm";
    QString expected = _inputPath + "ChangesetDeriveUpdateVersion_expected.osm";

    ChangesetCreator(false, "").create(output, reference, secondary);

    HOOT_FILE_EQUALS(output, expected);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetCreatorTest, "quick");

}
