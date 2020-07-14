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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
  CPPUNIT_TEST(runXmlChangesetErrorFixTest);
  CPPUNIT_TEST(runXmlChangesetSplitDeleteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmApiChangesetTest()
    : HootTestFixture("test-files/io/OsmChangesetElementTest/",
                      UNUSED_PATH)
  {
  }

  void runXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "ToyTestAChangeset1.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runNonAsciiXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "DjiboutiNonAsciiTest.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "DjiboutiNonAsciiTestExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetJoinTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset.split.osc");
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetMergeExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetUpdateTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

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

    QString expectedText = FileUtils::readFully(_inputPath + "ToyTestAChangeset2.osc");

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 2));
  }

  void runXmlChangesetSplitTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

    //  Four elements max will divide the changeset into four changesets
    //  one for each way, with its corresponding nodes
    changeset.setMaxPushSize(4);

    QStringList expectedFiles;
    expectedFiles.append(_inputPath + "ToyTestASplit1.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit2.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit3.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit4.osc");

    QStringList updatedFiles;
    updatedFiles.append(_inputPath + "ToyTestASplit1.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit2.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit3.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit4.response.xml");

    long processed[] = { 4, 8, 37, 40 };

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
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");
    //  Split the ways to a max of 8 nodes per way
    changeset.splitLongWays(8);

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetSplitWayExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetErrorFixTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ChangesetErrorFixInput.osc");
    //  Fix the bad input changeset
    changeset.fixMalformedInput();

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetErrorFixExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    QString change = changeset.getChangesetString(info, 1);
    HOOT_STR_EQUALS(expectedText, change);

    QString error = changeset.getFailedChangesetString();
    QString expectedError = FileUtils::readFully(_inputPath + "ChangesetErrorFixErrors.osc");
    HOOT_STR_EQUALS(expectedError, error);
  }

  void runXmlChangesetSplitDeleteTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "DeleteSplit.osc");

    //  8 elements max will divide the changeset into 4 changesets
    changeset.setMaxPushSize(8);

    QStringList expectedFiles;
    expectedFiles.append(_inputPath + "DeleteSplit1.osc");
    expectedFiles.append(_inputPath + "DeleteSplit2.osc");
    expectedFiles.append(_inputPath + "DeleteSplit3.osc");
    expectedFiles.append(_inputPath + "DeleteSplit4.osc");

    QStringList updatedFiles;
    updatedFiles.append(_inputPath + "DeleteSplit1.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit2.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit3.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit4.response.xml");

    long processed[] = { 8, 8, 16, 24 };

    int index = 0;
    while (!changeset.isDone())
    {
      ChangesetInfoPtr info1(new ChangesetInfo());
      changeset.calculateChangeset(info1);
      QString expectedText1 = FileUtils::readFully(expectedFiles[index]);
      HOOT_STR_EQUALS(expectedText1, changeset.getChangesetString(info1, index + 1));

      ChangesetInfoPtr info2(new ChangesetInfo());
      changeset.calculateChangeset(info2);
      QString expectedText2 = FileUtils::readFully(expectedFiles[index + 1]);
      HOOT_STR_EQUALS(expectedText2, changeset.getChangesetString(info2, index + 2));

      QString updatedText1 = FileUtils::readFully(updatedFiles[index]);
      changeset.updateChangeset(updatedText1);
      CPPUNIT_ASSERT_EQUAL(processed[index], changeset.getProcessedCount());

      QString updatedText2 = FileUtils::readFully(updatedFiles[index + 1]);
      changeset.updateChangeset(updatedText2);
      CPPUNIT_ASSERT_EQUAL(processed[index + 1], changeset.getProcessedCount());

      index += 2;
    }
    CPPUNIT_ASSERT_EQUAL(24L, changeset.getTotalElementCount());
    CPPUNIT_ASSERT_EQUAL(23L, changeset.getTotalNodeCount());
    CPPUNIT_ASSERT_EQUAL(1L, changeset.getTotalWayCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalRelationCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalCreateCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalModifyCount());
    CPPUNIT_ASSERT_EQUAL(24L, changeset.getTotalDeleteCount());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "current");

}
