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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

import org.apache.commons.exec.CommandLine;
import org.apache.commons.lang3.StringUtils;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.command.ExternalCommandRunnerImpl;

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
                "hoot::DualHighwaySplitter;hoot::HighwayImpliedDividedMarker;" +
                "hoot::DuplicateNameRemover;hoot::SmallHighwayMerger;" +
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

        String expectedCommand = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        assertEquals(expectedCommand, conflateCommand.getCommand());

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, conflateCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(conflateCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        String command = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(command.startsWith("[-D, writer.include.conflate.score.tags=false, "));
        assertTrue(command.contains("-D, hootapi.db.writer.overwrite.map=true, "));
        assertTrue(command.contains("-D, writer.text.status=true, "));
        assertTrue(command.contains("-D, api.db.email=test@test.com, "));
        assertTrue(command.endsWith("-D, \"map.cleaner.transforms=hoot::ReprojectToPlanarOp;" +
                                    "hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                                    "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                                    "hoot::DualHighwaySplitter;hoot::HighwayImpliedDividedMarker;" +
                                    "hoot::DuplicateNameRemover;hoot::SmallHighwayMerger;" +
                                    "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                                    "hoot::NoInformationElementRemover\"]"));
        assertTrue(command.contains("-D, job.id="));

        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads", conflateCommand.getSubstitutionMap().get("INPUT1"));
        assertEquals("hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads", conflateCommand.getSubstitutionMap().get("INPUT2"));
    }

    @Test
    @Category(UnitTest.class)
    public void testAttribute() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // captures expectation for attribute

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

        String expectedCommand = "hoot conflate --error -C AttributeConflation.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testNetworkAlgorithm() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for network...
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
        conflateParams.setConflateAlgorithm("Network");
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -C NetworkAlgorithm.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testHorizontal() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for network...
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
        conflateParams.setConflationType("Horizontal");
        String debugLevel = "trace";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --trace -C HorizontalConflation.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testNetwork() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for network...
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
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -C NetworkAlgorithm.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testDisabledFeaturesAndCleaningOptions() {
        String jobId = UUID.randomUUID().toString();

        // handles disabled features...
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
        conflateParams.setConflateAlgorithm("Network");
        conflateParams.setDisabledFeatures(Arrays.asList("Roads", "Pois"));
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

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
        conflateParams.setCleaningOpts(Arrays.asList("DualHighwaySplitter"));
        conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());
        options = conflateCommand.getSubstitutionMap().get("HOOT_OPTIONS").toString();
        assertTrue(options.contains("\"map.cleaner.transforms-=hoot::DualHighwaySplitter\""));

    }

    @Test
    @Category(UnitTest.class)
    public void testDifferential() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for network...
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("differential");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(true);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflationType("Differential");
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -C DifferentialConflation.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d --differential --stats";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testDifferentialWithTags() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for Differential with tags...
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("differential-tags");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflationType("Differential w/Tags");
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -C DifferentialConflation.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d --differential --include-tags";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testReference() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for reference conflation type
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(true);
        conflateParams.setReferenceLayer("1");
        conflateParams.setHoot2(true);
        conflateParams.setConflationType("Reference");
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -C ReferenceConflation.conf -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d --stats";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testDefault() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for no Conflation Type which should be reference...
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(true);
        conflateParams.setHoot2(true);
        String debugLevel = "error";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --error -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d --stats";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testReferenceLayer2() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for setting reference layer to 2
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(true);
        conflateParams.setReferenceLayer("2");
        conflateParams.setHoot2(true);
        String debugLevel = "info";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --info -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com -D tag.merger.default=hoot::OverwriteTag1Merger hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d --stats";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    @Test
    @Category(UnitTest.class)
    public void testHoot2AdvOpts() {
        String jobId = "38c74757-d444-49aa-b746-3ee29fc49cf7";

        // handles case for setting reference layer to 2
        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setConflationCommand("conflate");
        conflateParams.setInputType1("DB");
        conflateParams.setInput1("DcGisRoads");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2("DcTigerRoads");
        conflateParams.setOutputName("Merged_Roads_e0d");
        conflateParams.setCollectStats(false);
        conflateParams.setHoot2(true);
        Map<String,String> advOpts = new HashMap<>();
        advOpts.put("BuildingKeepMoreComplexGeometryWhenAutoMerging", "false");
        conflateParams.setHoot2AdvOptions(advOpts);
        String debugLevel = "info";
        ConflateCommand conflateCommand = new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

        String expectedCommand = "hoot conflate --info -D writer.include.conflate.score.tags=false -D hootapi.db.writer.overwrite.map=true -D writer.text.status=true -D job.id=38c74757-d444-49aa-b746-3ee29fc49cf7 -D api.db.email=test@test.com -D building.keep.more.complex.geometry.when.auto.merging=false hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcGisRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/DcTigerRoads hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/Merged_Roads_e0d";
        CommandLine actualCommand = ExternalCommandRunnerImpl.parse(conflateCommand.getCommand(), conflateCommand.getSubstitutionMap());
        assertEquals(expectedCommand, actualCommand.getExecutable() + " " + StringUtils.join(actualCommand.getArguments(), " "));

    }

    //{BuildingKeepMoreComplexGeometryWhenAutoMerging: false}

    @Test(expected = IllegalArgumentException.class)
    @Category(UnitTest.class)
    public void testInvalidType() {
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
        conflateParams.setConflationType("Foo");

        String debugLevel = "error";

        new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

    }

    @Test(expected = IllegalArgumentException.class)
    @Category(UnitTest.class)
    public void testInvalidAlgorithm() {
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
        conflateParams.setConflateAlgorithm("Attribute");

        String debugLevel = "error";

        new ConflateCommandFactory().build(jobId, conflateParams, debugLevel, this.getClass());

    }}
