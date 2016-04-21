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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/ops/CookieCutterOp.h>
#include <hoot/core/ops/AddHilbertReviewSortOrderOp.h>
#include <hoot/core/ops/BuildingOutlineRemoveOp.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/RemoveDuplicateReviewsOp.h>
#include <hoot/core/ops/RemoveReviewsByEidOp.h>
#include <hoot/core/ops/ReprojectToGeographicOp.h>
#include <hoot/core/ops/ReprojectToPlanarOp.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/WaySplitterOp.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/conflate/MapCleaner.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class GeneralOpTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(GeneralOpTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    QDataStream dataStream;
    string className = "";

    // AddHilbertReviewSortOrderOp
    AddHilbertReviewSortOrderOp addHilbertReviewSortOrderOp;
    className = addHilbertReviewSortOrderOp.getClassName();
    HOOT_STR_EQUALS("hoot::AddHilbertReviewSortOrderOp", className);

    // BuildingOutlineRemoveOp
    BuildingOutlineRemoveOp buildingOutlineRemoveOp;
    className = buildingOutlineRemoveOp.getClassName();
    HOOT_STR_EQUALS("hoot::BuildingOutlineRemoveOp", className);
    buildingOutlineRemoveOp.readObject(dataStream);
    buildingOutlineRemoveOp.writeObject(dataStream);

    // BuildingOutlineUpdateOp
    BuildingOutlineUpdateOp buildingOutlineUpdateOp;
    className = buildingOutlineUpdateOp.getClassName();
    HOOT_STR_EQUALS("hoot::BuildingOutlineUpdateOp", className);
    buildingOutlineUpdateOp.readObject(dataStream);
    buildingOutlineUpdateOp.writeObject(dataStream);

    // BuildingPartMergeOp
    BuildingPartMergeOp buildingPartMergeOp;
    className = buildingPartMergeOp.getClassName();
    HOOT_STR_EQUALS("hoot::BuildingPartMergeOp", className);
    buildingPartMergeOp.readObject(dataStream);
    buildingPartMergeOp.writeObject(dataStream);

    // RemoveDuplicateReviewsOp
    RemoveDuplicateReviewsOp removeDuplicateReviewsOp;
    className = removeDuplicateReviewsOp.getClassName();
    HOOT_STR_EQUALS("hoot::RemoveDuplicateReviewsOp", className);
    removeDuplicateReviewsOp.readObject(dataStream);
    removeDuplicateReviewsOp.writeObject(dataStream);

    // RemoveReviewsByEidOp
    RemoveReviewsByEidOp removeReviewsByEidOp;
    className = removeReviewsByEidOp.className();
    HOOT_STR_EQUALS("hoot::RemoveReviewsByEidOp", className);

    // ReprojectToGeographicOp
    ReprojectToGeographicOp reprojectToGeographicOp;
    className = reprojectToGeographicOp.getClassName();
    HOOT_STR_EQUALS("hoot::ReprojectToGeographicOp", className);
    reprojectToGeographicOp.readObject(dataStream);
    reprojectToGeographicOp.writeObject(dataStream);

    // ReprojectToPlanarOp
    ReprojectToPlanarOp reprojectToPlanarOp;
    className = reprojectToPlanarOp.getClassName();
    HOOT_STR_EQUALS("hoot::ReprojectToPlanarOp", className);
    reprojectToPlanarOp.readObject(dataStream);
    reprojectToPlanarOp.writeObject(dataStream);

    // SuperfluousNodeRemover
    SuperfluousNodeRemover superfluousNodeRemover;
    className = superfluousNodeRemover.getClassName();
    HOOT_STR_EQUALS("hoot::SuperfluousNodeRemover", className);

    // WaySplitterOp
    WaySplitterOp waySplitterOp;
    className = waySplitterOp.getClassName();
    HOOT_STR_EQUALS("hoot::WaySplitterOp", className);
    waySplitterOp.readObject(dataStream);
    waySplitterOp.writeObject(dataStream);
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeneralOpTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(GeneralOpTest, "current");


