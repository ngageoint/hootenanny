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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmApiChangesetTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiChangesetTest);
  CPPUNIT_TEST(runXmlChangesetTest);
  CPPUNIT_TEST(runNonAsciiXmlChangesetTest);
  CPPUNIT_TEST(runXmlChangesetJoinTest);
  CPPUNIT_TEST(runXmlChangesetUpdateTest);
  CPPUNIT_TEST(runXmlChangesetSplitTest);
  CPPUNIT_TEST(runXmlChangesetSplitWayTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ToyTestAChangeset1.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runNonAsciiXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/DjiboutiNonAsciiTest.osc");

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/DjiboutiNonAsciiTestExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetJoinTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset.split.osc");
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc");

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ChangesetMergeExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetUpdateTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    QString update =
        "<diffResult generator=\"OpenStreetMap Server\" version=\"0.6\">\n"
        "  <node old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <node old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "  <node old_id=\"-32\" new_id=\"32\" new_version=\"1\"/>\n"
        "  <node old_id=\"-7\" new_id=\"7\" new_version=\"1\"/>\n"
        "  <node old_id=\"-8\" new_id=\"8\" new_version=\"1\"/>\n"
        "  <node old_id=\"-33\" new_id=\"33\" new_version=\"1\"/>\n"
        "  <way old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <way old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "</diffResult>";

    changeset.updateChangeset(update);

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ToyTestAChangeset2.osc");

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 2));
  }

  void runXmlChangesetSplitTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");

    changeset.setMaxSize(10);

    QStringList expectedFiles;
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit1.osc");
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit2.osc");
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit3.osc");
    expectedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit4.osc");

    QStringList updatedFiles;
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit1.response.xml");
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit2.response.xml");
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit3.response.xml");
    updatedFiles.append("test-files/io/OsmChangesetElementTest/ToyTestASplit4.response.xml");

    long processed[] = { 10, 20, 30, 40 };

    ChangesetInfoPtr info;
    int index = 0;
    while (!changeset.isDone())
    {
      info.reset(new ChangesetInfo());
      changeset.calculateChangeset(info);

      QString expectedText = FileUtils::readFully(expectedFiles[index]);

      HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, index + 1));

      QString updatedText = FileUtils::readFully(updatedFiles[index]);
      changeset.updateChangeset(updatedText);

      CPPUNIT_ASSERT_EQUAL(processed[index], changeset.getProcessedCount());

      ++index;
    }
    CPPUNIT_ASSERT_EQUAL(40L, changeset.getTotalElementCount());
    CPPUNIT_ASSERT_EQUAL(36L, changeset.getTotalNodeCount());
    CPPUNIT_ASSERT_EQUAL(4L, changeset.getTotalWayCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalRelationCount());
    CPPUNIT_ASSERT_EQUAL(40L, changeset.getTotalCreateCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalModifyCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalDeleteCount());
  }

  void runXmlChangesetSplitWayTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmChangesetElementTest/ToyTestAInput.osc");
    //  Split the ways to a max of 8 nodes per way
    changeset.splitLongWays(8);

    QString expectedText = FileUtils::readFully("test-files/io/OsmChangesetElementTest/ChangesetSplitWayExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "current");

}
