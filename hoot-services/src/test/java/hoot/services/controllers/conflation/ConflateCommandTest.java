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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
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

        String expectedCommand = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        assertEquals(expectedCommand, conflateCommand.getCommand());

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, conflateCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        String command = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(command.startsWith("[-D, convert.ops=hoot::DecomposeBuildingRelationsVisitor, "));
        assertTrue(command.contains("-D, writer.include.conflate.score.tags=false, "));
        assertTrue(command.contains("-D, hootapi.db.writer.overwrite.map=true, "));
        assertTrue(command.contains("-D, writer.text.status=true, "));
        assertTrue(command.contains("-D, api.db.email=test@test.com, "));
        assertTrue(command.endsWith("-D, \"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                                    "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                                    "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                                    "hoot::DualWaySplitter;hoot::ImpliedDividedMarker;" +
                                    "hoot::DuplicateNameRemover;hoot::SmallWayMerger;" +
                                    "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                                    "hoot::NoInformationElementRemover\"]"));
        assertTrue(command.contains("-D, hootapi.db.writer.job.id="));

        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads", conflateCommand.getSubstitutionMap().get("INPUT1"));
        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads", conflateCommand.getSubstitutionMap().get("INPUT2"));
    }

    @Test
    @Category(UnitTest.class)
    public void testCreateConflateCommandHoot2() {
        String jobId = UUID.randomUUID().toString();

        // captures expectation for attribute, reference, horizontal...

        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflationType("Attribute");

        String debugLevel = "error";

        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        assertEquals(jobId, conflateCommand.getJobId());
        assertEquals(true, conflateCommand.getTrackable());
        assertNotNull(conflateCommand.getSubstitutionMap());
        assertNotNull(conflateCommand.getWorkDir());
        assertNotNull(conflateCommand.getCommand());

        String expectedCommand = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf -C ${CONFLATION_TYPE} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        assertEquals(expectedCommand, conflateCommand.getCommand());

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("CONFLATION_TYPE"));
        assertTrue(conflateCommand.getSubstitutionMap().get("CONFLATION_TYPE").toString().equals("AttributeConflation.conf"));

        // handles case for network...
        jobId = UUID.randomUUID().toString();
        conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflateAlgorithm("Network");
        debugLevel = "error";
        conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        expectedCommand = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf -C ${CONFLATION_ALGORITHM} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        assertEquals(expectedCommand, conflateCommand.getCommand());

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("CONFLATION_ALGORITHM"));
        assertTrue(conflateCommand.getSubstitutionMap().get("CONFLATION_ALGORITHM").toString().equals("NetworkAlgorithm.conf"));

        // handles disabled features...
        conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflateAlgorithm("Network");
        conflateParams.setDisabledFeatures(Arrays.asList("Roads", "Pois"));
        conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String options = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(options.contains("match"));
        assertTrue(options.contains("merger"));


        Map<String, String> hoot2Opts = new HashMap<>();

        hoot2Opts.put("HighwayMergeTagsOnly", "true");
        hoot2Opts.put("AddressAdditionalTagKeys", "[foo=bar,blim=blam]");
        conflateParams.setHoot2AdvOptions(hoot2Opts);

        conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());
        options = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(options.contains("\"address.additional.tag.keys=foo=bar;blim=blam\""));
        assertTrue(options.contains("\"highway.merge.tags.only=true\""));

        // handles cleaning options...
        conflateParams.setCleaningOpts(Arrays.asList("DualWaySplitter"));
        conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());
        options = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(options.contains("\"map.cleaner.transforms-=hoot::DualWaySplitter\""));

    }

    @Test(expected = IllegalArgumentException.class)
    @Category(UnitTest.class)
    public void testHoot2Exceptions() {
        // illustrates command will not be executed if alg or conflate type are not valid...

        String jobId = UUID.randomUUID().toString();

        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflationType("Network");

        String debugLevel = "error";

        new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        conflateParams.setConflationCommand(null);
        conflateParams.setConflationType("Attribute");

        new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

    }
}
