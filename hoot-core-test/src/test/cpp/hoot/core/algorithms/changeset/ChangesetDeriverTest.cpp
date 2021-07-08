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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class ChangesetDeriverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetDeriverTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(disableRefDeleteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetDeriverTest() :
  HootTestFixture("test-files/algorithms/changeset/ChangesetDeriverTest/", UNUSED_PATH)
  {
  }

  QMap<Change::ChangeType, QList<long>> _getChangeset(
    const QString& input1, const QString& input2, const bool allowRefDelete = true)
  {
    OsmMapPtr map1 = std::make_shared<OsmMap>();
    // each dataset needs to have a different input status
    OsmMapReaderFactory::read(map1, input1, true, Status::Unknown1);

    OsmMapPtr map2 = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map2, input2, true, Status::Unknown2);

    // input has to be sorted
    InMemoryElementSorterPtr map1SortedElements(new InMemoryElementSorter(map1));
    InMemoryElementSorterPtr map2SortedElements(new InMemoryElementSorter(map2));

    // stream out the changeset results grouped by change type
    ChangesetDeriverPtr changesetDiff(new ChangesetDeriver(map1SortedElements, map2SortedElements));
    changesetDiff->setAllowDeletingReferenceFeatures(allowRefDelete);
    QMap<Change::ChangeType, QList<long>> changeTypeToIds;
    while (changesetDiff->hasMoreChanges())
    {
      const Change change = changesetDiff->readNextChange();
      LOG_VART(change.toString());
      changeTypeToIds[change.getType()].append(change.getElement()->getElementId().getId());
    }

    return changeTypeToIds;
  }

  void runTest()
  { 
    const QMap<Change::ChangeType, QList<long>> changeTypeToIds =
      _getChangeset(_inputPath + "Map1.osm", _inputPath + "Map2.osm");

    HOOT_STR_EQUALS("[2]{-7, -2}", changeTypeToIds[Change::Create]);
    HOOT_STR_EQUALS("[1]{-4}", changeTypeToIds[Change::Modify]);
    HOOT_STR_EQUALS("[2]{-6, -1}", changeTypeToIds[Change::Delete]);
  }

  void disableRefDeleteTest()
  {
    const QMap<Change::ChangeType, QList<long>> changeTypeToIds =
      _getChangeset(
        _inputPath + "disableRefDeleteTest-in-1.osm", _inputPath + "disableRefDeleteTest-in-2.osm",
        false);

    // These checks could be a bit more granular but good enough for now while we're still
    // experimenting with this feature.
    HOOT_STR_EQUALS(99, changeTypeToIds[Change::Create].size());
    HOOT_STR_EQUALS(0, changeTypeToIds[Change::Modify].size());
    HOOT_STR_EQUALS(0, changeTypeToIds[Change::Delete].size());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetDeriverTest, "quick");

}
