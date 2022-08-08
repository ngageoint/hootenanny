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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;


import static org.junit.Assert.*;

import java.io.File;
import java.util.UUID;

import org.junit.Test;


public class BasemapCommandTest {

    @Test
    public void testBasemapCommandBasic() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        String basemapName = "basemap";
        File inputFile = new File("input");
        File tileOutputDir = new File("tileOutputDir");
        Boolean verboseOutput = true;
        String projection = "proj";

        BasemapCommand basemapCommand = new BasemapCommand(jobId, basemapName, inputFile, projection, tileOutputDir,  verboseOutput, caller);

        assertEquals(jobId, basemapCommand.getJobId());
        assertEquals(true, basemapCommand.getTrackable());
        assertNotNull(basemapCommand.getSubstitutionMap());
        assertNotNull(basemapCommand.getWorkDir());
        assertNotNull(basemapCommand.getCommand());

        String expectedCommand = "/usr/local/bin/gdal2tiles.py ${VERBOSE} -s ${PROJECTION} -w ${WEBVIEWER} " +
                "-t ${TITLE} -z ${ZOOM} ${INPUT_FILE} ${TILE_OUTPUT_DIR}";
        assertEquals(expectedCommand, basemapCommand.getCommand());

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("VERBOSE"));
        assertEquals("-v", basemapCommand.getSubstitutionMap().get("VERBOSE"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("PROJECTION"));
        assertEquals("proj", basemapCommand.getSubstitutionMap().get("PROJECTION"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("WEBVIEWER"));
        assertEquals("none", basemapCommand.getSubstitutionMap().get("WEBVIEWER"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("TITLE"));
        assertTrue(basemapCommand.getSubstitutionMap().get("TITLE").toString().endsWith("input"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("ZOOM"));
        assertEquals("0-20", basemapCommand.getSubstitutionMap().get("ZOOM"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("INPUT_FILE"));
        assertTrue(basemapCommand.getSubstitutionMap().get("INPUT_FILE").toString().endsWith("input"));

        assertTrue(basemapCommand.getSubstitutionMap().containsKey("TILE_OUTPUT_DIR"));
        assertTrue(basemapCommand.getSubstitutionMap().get("TILE_OUTPUT_DIR").toString().endsWith("tileOutputDir"));
    }
}