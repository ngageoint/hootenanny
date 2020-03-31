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
package hoot.services.controllers.clipping;


import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.junit.Test;


public class ClipDatasetCommandTest {

    @Test
    public void testClipDatasetCommandBasic() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        String input = "input";
        String output = "output";
        String bounds = "38,-105,39,-104";

        ClipDatasetParams params = new ClipDatasetParams();
        params.setInputName(input);
        params.setOutputName(output);
        params.setBounds(bounds);

        ClipDatasetCommand clipDatasetCommand = new ClipDatasetCommand(jobId, params, debugLevel, caller, null);

        List<String> options = new LinkedList<>();
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, clipDatasetCommand.getJobId());
        assertEquals(true, clipDatasetCommand.getTrackable());
        assertNotNull(clipDatasetCommand.getSubstitutionMap());
        assertNotNull(clipDatasetCommand.getWorkDir());
        assertNotNull(clipDatasetCommand.getCommand());

        String expectedCommand = "hoot.bin crop --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT} ${BOUNDS}";
        assertEquals(expectedCommand, clipDatasetCommand.getCommand());

        assertTrue(clipDatasetCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, clipDatasetCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(clipDatasetCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, clipDatasetCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        assertTrue(clipDatasetCommand.getSubstitutionMap().containsKey("INPUT"));
        assertEquals(HOOTAPI_DB_URL + "/" + input, clipDatasetCommand.getSubstitutionMap().get("INPUT"));

        assertTrue(clipDatasetCommand.getSubstitutionMap().containsKey("OUTPUT"));
        assertEquals(HOOTAPI_DB_URL + "/" + output, clipDatasetCommand.getSubstitutionMap().get("OUTPUT"));

        assertTrue(clipDatasetCommand.getSubstitutionMap().containsKey("BOUNDS"));
        assertEquals(bounds, clipDatasetCommand.getSubstitutionMap().get("BOUNDS"));
    }
}
