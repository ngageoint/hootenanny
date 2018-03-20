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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;


import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.*;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.apache.xpath.XPathAPI;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.IntegrationTest;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommandRunner;
import hoot.services.command.ExternalCommandRunnerImpl;
import hoot.services.utils.MapUtils;
import hoot.services.utils.XmlDocumentBuilder;


public class DeriveChangesetCommandTest {

    @Test
    public void testDeriveChangesetNoSQLCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setBounds(aoi);

        DeriveChangesetCommand deriveChangesetCommand = new DeriveChangesetCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = deriveChangesetCommand.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("osm.changeset.sql.file.writer.generate.new.ids=false");
        options.add("changeset.user.id=" + exportParams.getUserId());

        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, deriveChangesetCommand.getJobId());
        assertEquals(true, deriveChangesetCommand.getTrackable());
        assertNotNull(deriveChangesetCommand.getSubstitutionMap());
        assertNotNull(deriveChangesetCommand.getWorkDir());
        assertNotNull(deriveChangesetCommand.getCommand());

        String expectedCommand = "hoot changeset-derive --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT_PATH}";
        assertEquals(expectedCommand, deriveChangesetCommand.getCommand());

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, deriveChangesetCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, deriveChangesetCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("OSMAPI_DB_URL"));
        assertEquals(OSMAPI_DB_URL, deriveChangesetCommand.getSubstitutionMap().get("OSMAPI_DB_URL"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("INPUT"));
        assertEquals(exportParams.getInput(), deriveChangesetCommand.getSubstitutionMap().get("INPUT"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                exportParams.getOutputName() + "." + exportParams.getOutputType()).getAbsolutePath();

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("OUTPUT_PATH"));
        assertEquals(expectedOutputPath, deriveChangesetCommand.getSubstitutionMap().get("OUTPUT_PATH"));
    }

    @Test
    public void testDeriveChangesetWithSQLCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setInput("map-with-id-" + mapId);
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setAppend(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setBounds(aoi);
        exportParams.setUserId(String.valueOf(userId));

        DeriveChangesetCommand deriveChangesetCommand = new DeriveChangesetCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = deriveChangesetCommand.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("osm.changeset.sql.file.writer.generate.new.ids=false");
        options.add("changeset.user.id=" + exportParams.getUserId());

        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, deriveChangesetCommand.getJobId());
        assertEquals(true, deriveChangesetCommand.getTrackable());
        assertNotNull(deriveChangesetCommand.getSubstitutionMap());
        assertNotNull(deriveChangesetCommand.getWorkDir());
        assertNotNull(deriveChangesetCommand.getCommand());

        String expectedCommand = "hoot changeset-derive --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSMAPI_DB_URL} " +
                "${INPUT} ${CHANGESET_OUTPUT_PATH} ${OSMAPI_DB_URL}";
        assertEquals(expectedCommand, deriveChangesetCommand.getCommand());

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, deriveChangesetCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, deriveChangesetCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("OSMAPI_DB_URL"));
        assertEquals(OSMAPI_DB_URL, deriveChangesetCommand.getSubstitutionMap().get("OSMAPI_DB_URL"));

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("INPUT"));
        assertEquals(exportParams.getInput(), deriveChangesetCommand.getSubstitutionMap().get("INPUT"));

        String expectedChangesetOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                "changeset-" + jobId + ".osc.sql").getAbsolutePath();

        assertTrue(deriveChangesetCommand.getSubstitutionMap().containsKey("CHANGESET_OUTPUT_PATH"));
        assertEquals(expectedChangesetOutputPath, deriveChangesetCommand.getSubstitutionMap().get("CHANGESET_OUTPUT_PATH"));
    }

    // This test is no longer valid (05/04/2017).  Should be most likely removed
    @Ignore
    @Test
    @Category(IntegrationTest.class)
    public void testExportToChangescript() throws Exception {
        File workDir = null;
        try {
            String jobId = UUID.randomUUID().toString();
            String debugLevel = "error";
            Class<?> caller = this.getClass();
            String aoi = "-104.8192,38.8162,-104.6926,38.9181";

            workDir = new File(TEMP_OUTPUT_PATH, jobId);
            FileUtils.forceMkdir(workDir);
            File outputFile = new File(new File(TEMP_OUTPUT_PATH, jobId),jobId + ".osc");

            ExportParams exportParams = new ExportParams();
            //exportParams.setInput(HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/AllDataTypesA.osm");
            //exportParams.setInput(HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/ExportJobResourceTestAdtConflated.osm");
            exportParams.setOutputName(jobId);
            exportParams.setAppend(false);
            exportParams.setTextStatus(false);
            exportParams.setInputType("file");
            exportParams.setOutputType("osc");
            exportParams.setBounds(aoi);

            DeriveChangesetCommand exportCommand = new DeriveChangesetCommand(jobId, exportParams, debugLevel, caller);
            ExternalCommandRunner externalCommandRunner = new ExternalCommandRunnerImpl();
            CommandResult commandResult = externalCommandRunner.exec(exportCommand.getCommand(), exportCommand.getSubstitutionMap(),
                    exportCommand.getJobId(), this.getClass().getName(), exportCommand.getWorkDir(), false);

            //verify output file - we're not going to do an exact diff on it to avoid a dependency on
            //core changeset generation logic which may change in the future.  just going to check for
            //non-zero element counts on create/delete, which is good enough indication that the
            //changeset generated without a failure
            Document actualChangesetDoc = XmlDocumentBuilder.parse(FileUtils.readFileToString(outputFile, "UTF-8"));
            assert (XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange").getLength() == 1);
            assert (XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange/create").getLength() > 0);
            //assert (XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange/delete").getLength() > 0);
        }
        finally {
            FileUtils.deleteQuietly(workDir);
        }
    }
}