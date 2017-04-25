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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;


import static hoot.services.HootProperties.*;
import static org.junit.Assert.*;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.apache.commons.io.FileUtils;
import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.IntegrationTest;
import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommandRunner;
import hoot.services.command.ExternalCommandRunnerImpl;
import hoot.services.utils.XmlDocumentBuilder;


public class ExportOSCCommandTest {

    @Test
    public void testExportOSCCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setInput1("input1");
        exportParams.setInput1("input2");
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setBounds(aoi);

        ExportOSCCommand exportCommand = new ExportOSCCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = exportCommand.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("osm.changeset.sql.file.writer.generate.new.ids=false");

        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot derive-changeset --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT_PATH}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT1"));
        assertEquals(exportParams.getInput1(), exportCommand.getSubstitutionMap().get("INPUT1"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT2"));
        assertEquals(exportParams.getInput2(), exportCommand.getSubstitutionMap().get("INPUT2"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                exportParams.getOutputName() + "." + exportParams.getOutputType()).getAbsolutePath();

        assertTrue(exportCommand.getSubstitutionMap().containsKey("OUTPUT_PATH"));
        assertEquals(expectedOutputPath, exportCommand.getSubstitutionMap().get("OUTPUT_PATH"));
    }

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
            exportParams.setInput1(HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/AllDataTypesA.osm");
            exportParams.setInput2(HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/ExportJobResourceTestAdtConflated.osm");
            exportParams.setOutputName(jobId);
            exportParams.setAppend(false);
            exportParams.setTextStatus(false);
            exportParams.setInputType("file");
            exportParams.setOutputType("osc");
            exportParams.setBounds(aoi);

            ExportOSCCommand exportCommand = new ExportOSCCommand(jobId, exportParams, debugLevel, caller);
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
            assert (XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange/delete").getLength() > 0);
        }
        finally {
            FileUtils.deleteQuietly(workDir);
        }
    }
}