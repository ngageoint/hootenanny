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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static org.junit.Assert.*;

import java.util.LinkedList;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ConflateCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testCreateConflateCommand() {
        String jobId = UUID.randomUUID().toString();

        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setUserEmail("test@test.com");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");

        List<String> advancedOptions = new LinkedList<>();
        advancedOptions.add("\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                "hoot::DualWaySplitter;hoot::ImpliedDividedMarker;" +
                "hoot::DuplicateNameRemover;hoot::SmallWayMerger;" +
                "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                "hoot::NoInformationElementRemover\"");

        conflateParams.setAdvancedOptions(advancedOptions.stream().collect(Collectors.joining(" ")));

        String debugLevel = "error";

        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        assertEquals(jobId, conflateCommand.getJobId());
        assertEquals(true, conflateCommand.getTrackable());
        assertNotNull(conflateCommand.getSubstitutionMap());
        assertNotNull(conflateCommand.getWorkDir());
        assertNotNull(conflateCommand.getCommand());

        String expectedCommand = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${STATS}";
        assertEquals(expectedCommand, conflateCommand.getCommand());

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, conflateCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals("[-D, convert.ops=hoot::DecomposeBuildingRelationsVisitor, " +
                              "-D, writer.include.conflate.score.tags=false, " +
                              "-D, hootapi.db.writer.overwrite.map=true, " +
                              "-D, hootapi.db.writer.create.user=true, " +
                              "-D, writer.text.status=true, " +
                              "-D, api.db.email=test@test.com, " +
                              "-D, \"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                                    "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                                    "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                                    "hoot::DualWaySplitter;hoot::ImpliedDividedMarker;" +
                                    "hoot::DuplicateNameRemover;hoot::SmallWayMerger;" +
                                    "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                                    "hoot::NoInformationElementRemover\"]",
                conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString());

        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads", conflateCommand.getSubstitutionMap().get("INPUT1"));
        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads", conflateCommand.getSubstitutionMap().get("INPUT2"));
    }
}
