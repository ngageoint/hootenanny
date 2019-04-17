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
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>

namespace hoot
{

class ChangesetDeriverTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetDeriverTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetDeriverTest()
    : HootTestFixture("test-files/algorithms/changeset/ChangesetDeriverTest/",
                      UNUSED_PATH)
  {
  }

  void runTest()
  {
    CalculateHashVisitor2 hashVis;

    OsmMapPtr map1(new OsmMap());
    OsmMapReaderFactory::read(map1, _inputPath + "Map1.osm", true);
    map1->visitRw(hashVis);

    OsmMapPtr map2(new OsmMap());
    OsmMapReaderFactory::read(map2, _inputPath + "Map2.osm", true);
    map2->visitRw(hashVis);

    InMemoryElementSorterPtr map1SortedElements(new InMemoryElementSorter(map1));
    InMemoryElementSorterPtr map2SortedElements(new InMemoryElementSorter(map2));

    ChangesetDeriverPtr changesetDiff(
      new ChangesetDeriver(map1SortedElements, map2SortedElements));

    QMap<Change::ChangeType, QList<long> > changeTypeToIds;
    while (changesetDiff->hasMoreChanges())
    {
      const Change change = changesetDiff->readNextChange();
      LOG_VART(change.toString());
      changeTypeToIds[change.getType()].append(change.getElement()->getElementId().getId());
    }

    HOOT_STR_EQUALS("[2]{-7, -2}", changeTypeToIds[Change::Create]);
    HOOT_STR_EQUALS("[1]{-4}", changeTypeToIds[Change::Modify]);
    HOOT_STR_EQUALS("[2]{-6, -1}", changeTypeToIds[Change::Delete]);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetDeriverTest, "quick");

}
