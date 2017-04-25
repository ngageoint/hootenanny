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

import org.junit.Test;


public class ExportOSMCommandTest {

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

        ExportOSMCommand exportCommand = new ExportOSMCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = new LinkedList<>();
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");
        options.add("writer.text.status=true");

        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT_PATH}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("INPUT"));
        assertEquals(exportParams.getInput(), exportCommand.getSubstitutionMap().get("INPUT"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                exportParams.getOutputName() + "." + exportParams.getOutputType()).getAbsolutePath();

        assertTrue(exportCommand.getSubstitutionMap().containsKey("OUTPUT_PATH"));
        assertEquals(expectedOutputPath, exportCommand.getSubstitutionMap().get("OUTPUT_PATH"));
    }
}