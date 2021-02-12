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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.export;


import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.junit.Test;


public class ExportCommandTest {

    @Test
    public void testExportCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        ExportParams exportParams = new ExportParams();
        exportParams.setTranslation("translations/TDSv40.js");
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setIncludeHootTags(false);

        ExportCommand exportCommand = new ExportCommand(jobId, exportParams, debugLevel, caller, null);

        List<String> options = exportCommand.getCommonExportHootOptions();
        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot.bin convert --${DEBUG_LEVEL} " +
                "${HOOT_OPTIONS} ${INPUT_PATH} ${OUTPUT_PATH}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT_PATH"));
        assertEquals(exportParams.getInput(), exportCommand.getSubstitutionMap().get("INPUT_PATH"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                exportParams.getOutputName() + "." + exportParams.getOutputType()).getAbsolutePath();
        assertTrue(exportCommand.getSubstitutionMap().containsKey("OUTPUT_PATH"));
        assertEquals(expectedOutputPath, exportCommand.getSubstitutionMap().get("OUTPUT_PATH"));
    }

    @Test
    public void testExportOSMCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        ExportParams exportParams = new ExportParams();
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setIncludeHootTags(false);
        exportParams.setAppend(false);

        ExportCommand exportCommand = new ExportCommand(jobId, exportParams, debugLevel, caller, null);
        List<String> options = exportCommand.getCommonExportHootOptions();
        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot.bin convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT_PATH} ${OUTPUT_PATH}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT_PATH"));
        assertEquals(exportParams.getInput(), exportCommand.getSubstitutionMap().get("INPUT_PATH"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                exportParams.getOutputName() + "." + exportParams.getOutputType()).getAbsolutePath();

        assertTrue(exportCommand.getSubstitutionMap().containsKey("OUTPUT_PATH"));
        assertEquals(expectedOutputPath, exportCommand.getSubstitutionMap().get("OUTPUT_PATH"));
    }

}
