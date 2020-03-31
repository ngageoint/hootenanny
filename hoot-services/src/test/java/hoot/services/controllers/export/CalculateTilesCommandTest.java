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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class CalculateTilesCommandTest {
    @Test
    @Category(UnitTest.class)
    public void testCalculateTilesCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams jobParams = new ExportParams();
        jobParams.setOutputName("output");
        jobParams.setAppend(false);
        jobParams.setTextStatus(false);
        jobParams.setInput("input1;input2");
        jobParams.setInputType("file");
        jobParams.setOutputType("tiles");
        jobParams.setBounds(aoi);
        jobParams.setMaxNodeCountPerTile(1000);
        jobParams.setPixelSize(0.001);

        CalculateTilesCommand command = new CalculateTilesCommand(jobId, jobParams, debugLevel, caller, null);

        List<String> options = new LinkedList<>();
        options.add("api.db.email=test@test.com");
        options.add("convert.bounding.box=" + aoi);

        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, command.getJobId());
        assertEquals(true, command.getTrackable());
        assertNotNull(command.getSubstitutionMap());
        assertNotNull(command.getWorkDir());
        assertNotNull(command.getCommand());

        String expectedCommand = "hoot.bin node-density-tiles --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${OUTPUT} ${MAX_NODE_COUNT_PER_TILE} ${PIXEL_SIZE}";
        assertEquals(expectedCommand, command.getCommand());

        assertTrue(command.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, command.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(command.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, command.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(command.getSubstitutionMap().containsKey("INPUTS"));
        assertEquals(jobParams.getInput(), command.getSubstitutionMap().get("INPUTS"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                jobParams.getOutputName() + "." + jobParams.getOutputType()).getAbsolutePath();

        assertTrue(command.getSubstitutionMap().containsKey("OUTPUT"));
        assertEquals(expectedOutputPath, command.getSubstitutionMap().get("OUTPUT"));

        assertTrue(command.getSubstitutionMap().containsKey("MAX_NODE_COUNT_PER_TILE"));
        assertEquals("1000", command.getSubstitutionMap().get("MAX_NODE_COUNT_PER_TILE"));

        assertTrue(command.getSubstitutionMap().containsKey("PIXEL_SIZE"));
        assertEquals("0.001", command.getSubstitutionMap().get("PIXEL_SIZE"));
    }

    @Test
    @Category(UnitTest.class)
    public void testCalculateTilesCommandNoMaxNodeCountOrPixelSize() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams jobParams = new ExportParams();
        jobParams.setOutputName("output");
        jobParams.setAppend(false);
        jobParams.setTextStatus(false);
        jobParams.setInput("input1;input2");
        jobParams.setInputType("file");
        jobParams.setOutputType("tiles");
        jobParams.setBounds(aoi);

        CalculateTilesCommand command = new CalculateTilesCommand(jobId, jobParams, debugLevel, caller, null);

        List<String> options = new LinkedList<>();
        options.add("api.db.email=test@test.com");
        options.add("convert.bounding.box=" + aoi);

        List<String> hootOptions = new LinkedList<>();

        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, command.getJobId());
        assertEquals(true, command.getTrackable());
        assertNotNull(command.getSubstitutionMap());
        assertNotNull(command.getWorkDir());
        assertNotNull(command.getCommand());

        String expectedCommand = "hoot.bin node-density-tiles --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${OUTPUT}";
        assertEquals(expectedCommand, command.getCommand());

        assertTrue(command.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, command.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(command.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, command.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(command.getSubstitutionMap().containsKey("INPUTS"));
        assertEquals(jobParams.getInput(), command.getSubstitutionMap().get("INPUTS"));

        String expectedOutputPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                jobParams.getOutputName() + "." + jobParams.getOutputType()).getAbsolutePath();

        assertTrue(command.getSubstitutionMap().containsKey("OUTPUT"));
        assertEquals(expectedOutputPath, command.getSubstitutionMap().get("OUTPUT"));
    }

    @Test(expected = IllegalArgumentException.class)
    @Category(UnitTest.class)
    public void testCalculateTilesCommandNoMaxNodeCountOrPixelSize1() {
        try
        {
            String jobId = UUID.randomUUID().toString();
            String debugLevel = "error";
            Class<?> caller = this.getClass();
            String aoi = "-104.8192,38.8162,-104.6926,38.9181";

            ExportParams jobParams = new ExportParams();
            jobParams.setOutputName("output");
            jobParams.setAppend(false);
            jobParams.setTextStatus(false);
            jobParams.setInput("input1;input2");
            jobParams.setInputType("file");
            jobParams.setOutputType("tiles");
            jobParams.setBounds(aoi);
            jobParams.setMaxNodeCountPerTile(1000);

            new CalculateTilesCommand(jobId, jobParams, debugLevel, caller, null);
        }
        catch (IllegalArgumentException e) {
            assertTrue(e.getMessage().contains("If either max node count per tile or pixel size is specified, then both input parameters must be specified."));
            throw e;
        }
    }

    @Test(expected = IllegalArgumentException.class)
    @Category(UnitTest.class)
    public void testCalculateTilesCommandNoMaxNodeCountOrPixelSize2() {
        try
        {
            String jobId = UUID.randomUUID().toString();
            String debugLevel = "error";
            Class<?> caller = this.getClass();
            String aoi = "-104.8192,38.8162,-104.6926,38.9181";

            ExportParams jobParams = new ExportParams();
            jobParams.setOutputName("output");
            jobParams.setAppend(false);
            jobParams.setTextStatus(false);
            jobParams.setInput("input1;input2");
            jobParams.setInputType("file");
            jobParams.setOutputType("tiles");
            jobParams.setBounds(aoi);
            jobParams.setPixelSize(0.001);

            new CalculateTilesCommand(jobId, jobParams, debugLevel, caller, null);
        }
        catch (IllegalArgumentException e) {
            assertTrue(e.getMessage().contains("If either max node count per tile or pixel size is specified, then both input parameters must be specified."));
            throw e;
        }
    }
}
