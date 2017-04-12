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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ConflateCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testCreateConflateCommand() {

        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInput2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setUserEmail("test@test.com");
        conflateParams.setCollectStats(false);

        List<String> hootOptions = new LinkedList<>();
        hootOptions.add("-D");
        hootOptions.add("\"osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor\"");
        hootOptions.add("-D");
        hootOptions.add("\"conflate.add.score.tags=yes\"");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.overwrite.map=true");
        hootOptions.add("-D");
        hootOptions.add("hootapi.db.writer.create.user=true");
        hootOptions.add("-D");
        hootOptions.add("api.db.email=test@test.com");
        hootOptions.add("-D");
        hootOptions.add("\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                "hoot::DualWaySplitter;hoot::ImpliedDividedMarker;" +
                "hoot::DuplicateNameRemover;hoot::SmallWayMerger;" +
                "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                "hoot::NoInformationElementRemover\"");

        conflateParams.setAdvancedOptions(hootOptions.stream().collect(Collectors.joining(" ")));

        //ConflateCommand conflateCommand = new ConflateCommandFactory().build(conflateParams, null, "error", this.getClass());

    }
}