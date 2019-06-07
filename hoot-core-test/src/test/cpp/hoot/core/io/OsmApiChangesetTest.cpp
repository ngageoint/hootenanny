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
  CPPUNIT_TEST(runXmlChangesetErrorFixTest);
  CPPUNIT_TEST(runXmlChangesetFailureMatchesTest);
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
    changeset.setMaxSize(4);

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

  void runXmlChangesetFailureMatchesTest()
  {
    XmlChangeset changeset;

    bool found = false;
    long id = 0;
    long id2 = 0;
    ElementType::Type type = ElementType::Unknown;
    ElementType::Type type2 = ElementType::Unknown;
    QString hint;

    //  Test Placeholder failure with node in way
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder node not found for reference -145213 in way -5687";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-145213L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type);
    CPPUNIT_ASSERT_EQUAL(-5687L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type2);

    //  Test Placeholder failure with way in relation
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder Way not found for reference -12257 in relation -51";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-12257L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);
    CPPUNIT_ASSERT_EQUAL(-51L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type2);

    //  Test Placeholder failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type2);

    //  Test relation failure with no relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id  cannot be saved due to Relation with id 1707699";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(1707699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);

    //  Test relation failure with relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id 122 cannot be saved due to Way with id 7699";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(122L, id);
    CPPUNIT_ASSERT_EQUAL(7699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);

    //  Test relation failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "current");

}
