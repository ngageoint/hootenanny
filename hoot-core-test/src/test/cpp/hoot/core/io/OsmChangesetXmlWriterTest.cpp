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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include "TestOsmChangesetProvider.h"
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/OsmChangesetXmlWriter.h>

// Boost
using namespace boost;

// Qt
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class OsmChangesetXmlWriterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OsmChangesetXmlWriterTest);
    CPPUNIT_TEST(runSimpleTest);
    CPPUNIT_TEST(runSplitTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void runSimpleTest()
  {
    shared_ptr<ChangeSetProvider> changesetProvider(new TestOsmChangesetProvider(false));
    QDir().mkpath("test-output/io/OsmChangesetXmlWriterTest");
    OsmChangesetXmlWriter().write(
      "test-output/io/OsmChangesetXmlWriterTest/changeset.osc", changesetProvider);

    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmChangesetXmlWriterTest/changeset.osc"),
      TestUtils::readFile("test-output/io/OsmChangesetXmlWriterTest/changeset.osc"));
  }

  void runSplitTest()
  {
    shared_ptr<ChangeSetProvider> changesetProvider(new TestOsmChangesetProvider(false));
    QDir().mkpath("test-output/io/OsmChangesetXmlWriterTest");
    OsmChangesetXmlWriter writer;
    Settings testSettings = conf();
    testSettings.set("changeset.max.size", "5");
    writer.setConfiguration(testSettings);
    writer.write(
      "test-output/io/OsmChangesetXmlWriterTest/changeset.split.osc", changesetProvider);

    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmChangesetXmlWriterTest/changeset.split.osc"),
      TestUtils::readFile("test-output/io/OsmChangesetXmlWriterTest/changeset.split.osc"));
    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmChangesetXmlWriterTest/changeset-001.split.osc"),
      TestUtils::readFile("test-output/io/OsmChangesetXmlWriterTest/changeset-001.split.osc"));
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmChangesetXmlWriterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmChangesetXmlWriterTest, "quick");

}



