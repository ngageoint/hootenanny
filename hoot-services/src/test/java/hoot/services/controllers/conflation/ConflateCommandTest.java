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
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.CONFLATE_MAKEFILE_PATH;
import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ConflateCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testCreateConflateCommand() {
        String params = "{\"INPUT1_TYPE\":\"DB\"," +
                "\"INPUT1\":\"DcGisRoads\"," +
                "\"INPUT2_TYPE\":\"DB\"," +
                "\"INPUT2\":\"DcTigerRoads\"," +
                "\"OUTPUT_NAME\":\"Merged_Roads_e0d\"," +
                "\"CONFLATION_TYPE\":\"Reference\"," +
                "\"MATCH_THRESHOLD\":\"0.6\"," +
                "\"MISS_THRESHOLD\":\"0.6\"," +
                "\"USER_EMAIL\":\"test@test.com\"," +
                "\"COLLECT_STATS\":\"false\"}";

        ConflateCommand conflateCommand = new ConflateCommandFactory().build(params, null, this.getClass());

        String expectedCommandArgs =
                "[{\"MISS_THRESHOLD\":\"0.6\"}," +
                 "{\"INPUT1\":\"DcGisRoads\"}," +
                 "{\"CONFLATION_TYPE\":\"Reference\"}," +
                 "{\"USER_EMAIL\":\"test@test.com\"}," +
                 "{\"OUTPUT_NAME\":\"Merged_Roads_e0d\"}," +
                 "{\"MATCH_THRESHOLD\":\"0.6\"}," +
                 "{\"INPUT2\":\"DcTigerRoads\"}," +
                 "{\"COLLECT_STATS\":\"false\"}," +
                 "{\"INPUT1_TYPE\":\"DB\"}," +
                 "{\"INPUT2_TYPE\":\"DB\"}," +
                 "{\"DB_URL\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\"}," +
                 "{\"OSM_API_DB_URL\":\"osmapidb:\\/\\/${OSMAPI_DB_USER}:${OSMAPI_DB_PASSWORD}@${OSMAPI_DB_HOST}:${OSMAPI_DB_PORT}\\/${OSMAPI_DB_NAME}\"}]";

        String actualCommandArgs = conflateCommand.get("params").toString();

        assertEquals(expectedCommandArgs, actualCommandArgs);

        String actualCaller = (String) conflateCommand.get("caller");
        String expectedCaller = this.getClass().getName();

        assertEquals(expectedCaller, actualCaller);

        String actualExecType = (String) conflateCommand.get("exectype");
        String expectedExecType = "make";

        assertEquals(expectedExecType, actualExecType);

        String actualScriptName = (String) conflateCommand.get("exec");
        String expectedScriptName = CONFLATE_MAKEFILE_PATH;

        assertEquals(expectedScriptName, actualScriptName);
    }
}