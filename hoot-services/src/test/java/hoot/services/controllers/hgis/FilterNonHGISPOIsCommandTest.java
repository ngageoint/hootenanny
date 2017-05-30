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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.hgis;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.UUID;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class FilterNonHGISPOIsCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testFilterNonHgisPoisCommand() throws Exception {
        String jobId = UUID.randomUUID().toString();
        String srcMap = "testSrc1";
        String outputMap = "testOutput1";

        FilterNonHGISPOIsCommand filterNonHGISPOIsCommand = new FilterNonHGISPOIsCommand(jobId, srcMap, outputMap, this.getClass());

        assertEquals(jobId, filterNonHGISPOIsCommand.getJobId());
        assertEquals(true, filterNonHGISPOIsCommand.getTrackable());
        assertNotNull(filterNonHGISPOIsCommand.getSubstitutionMap());
        assertNotNull(filterNonHGISPOIsCommand.getWorkDir());
        assertNotNull(filterNonHGISPOIsCommand.getCommand());

        String expectedCommand = "${SCRIPT} ${SOURCE} ${OUTPUT}";
        assertEquals(expectedCommand, filterNonHGISPOIsCommand.getCommand());

        String expectedOutput = "hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/testOutput1";
        assertEquals(expectedOutput, filterNonHGISPOIsCommand.getSubstitutionMap().get("OUTPUT"));

        String expectedSource = "hootapidb://${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}/${HOOTAPI_DB_NAME}/testSrc1";
        assertEquals(expectedSource, filterNonHGISPOIsCommand.getSubstitutionMap().get("SOURCE"));

        assertTrue(filterNonHGISPOIsCommand.getSubstitutionMap().get("SCRIPT").toString().endsWith("RemoveNonHgisPois.js"));
    }
}