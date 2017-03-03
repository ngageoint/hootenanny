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


import static hoot.services.HootProperties.HGIS_PREPARE_FOR_VALIDATION_SCRIPT;
import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class HGISPrepareForValidationCommandTest {

    @Test
    @Category(UnitTest.class)
    public void testPrepareItemsForValidationCommand() throws Exception {
        HGISPrepareForValidationCommandFactory hgisPrepareForValidationCommandFactory = new HGISPrepareForValidationCommandFactory();
        HGISPrepareForValidationCommand command = hgisPrepareForValidationCommandFactory.build("testSrc1", "out1", this.getClass());

        String actualCaller = (String) command.get("caller");
        String expectedCaller = this.getClass().getName();
        assertEquals(expectedCaller, actualCaller);

        String actualExec = (String) command.get("exec");
        String expectedExec = HGIS_PREPARE_FOR_VALIDATION_SCRIPT;
        assertEquals(expectedExec, actualExec);

        String actualExectype = (String) command.get("exectype");
        String expectedExectype = "bash";
        assertEquals(expectedExectype, actualExectype);

        String expectedCommandArgs =
                "[{\"SOURCE\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\\/testSrc1\"}," +
                 "{\"OUTPUT\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\\/out1\"}]";

        String actualCommandArgs = command.get("params").toString();

        assertEquals(expectedCommandArgs, actualCommandArgs);
    }

}