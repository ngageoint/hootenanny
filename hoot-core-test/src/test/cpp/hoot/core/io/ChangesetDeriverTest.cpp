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
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ChangesetProvider.h>

#include "../TestUtils.h"

// Qt
#include <QDebug>

namespace hoot
{

class ChangesetDeriverTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ChangesetDeriverTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runTest()
    {
      boost::shared_ptr<OsmMap> map1(new OsmMap());
      OsmMapReaderFactory::read(map1, "test-files/io/ChangesetDeriverTest/Map1.osm", true);

      boost::shared_ptr<OsmMap> map2(new OsmMap());
      OsmMapReaderFactory::read(map2, "test-files/io/ChangesetDeriverTest/Map2.osm", true);

      ElementSorterPtr map1SortedElements(new ElementSorter(map1));
      ElementSorterPtr map2SortedElements(new ElementSorter(map2));

      ChangesetDeriverPtr changesetDiff(
        new ChangesetDeriver(map1SortedElements, map2SortedElements));

      QMap<Change::ChangeType, QList<long> > changeTypeToIds;
      while (changesetDiff->hasMoreChanges())
      {
        const Change change = changesetDiff->readNextChange();
        LOG_VART(change.toString());
        changeTypeToIds[change.type].append(change.e->getElementId().getId());
      }

      HOOT_STR_EQUALS("[2]{-7, -2}", changeTypeToIds[Change::Create]);
      HOOT_STR_EQUALS("[1]{-4}", changeTypeToIds[Change::Modify]);
      HOOT_STR_EQUALS("[3]{-6, -4, -1}", changeTypeToIds[Change::Delete]);
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetDeriverTest, "quick");

}
