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
package hoot.services.controllers.export;


import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.mockito.BDDMockito;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PowerMockIgnore;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.UnitTest;
import hoot.services.utils.DbUtils;
import hoot.services.utils.HootCustomPropertiesSetter;


@RunWith(PowerMockRunner.class)
@PrepareForTest({DbUtils.class, ExportCommand.class})
@PowerMockIgnore("javax.management.*")
public class ExportCommandTest {
    private static final Logger logger = LoggerFactory.getLogger(ExportCommandTest.class);

    private static final String MAP_NAME = "MyTestMap";
    private static final String EXPORT_TIME = "2016-05-04 10:15";
    private static final Map<String, String> tags = new HashMap<>();
    private static String originalTEMP_OUTPUT_PATH;

    @BeforeClass
    public static void beforeClass() throws Exception {
        originalTEMP_OUTPUT_PATH = TEMP_OUTPUT_PATH;
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", "/tmp");
    }

    @AfterClass
    public static void afterClass() throws Exception {
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", originalTEMP_OUTPUT_PATH);
    }

    @Before
    public void beforeTest() throws Exception {
        PowerMockito.mockStatic(DbUtils.class);

        BDDMockito.given(DbUtils.getMapIdByName(MAP_NAME)).willReturn(1L);
        BDDMockito.given(DbUtils.getMapsTableTags(1L)).willReturn(tags);
    }

    @Test
    @Category(UnitTest.class)
    public void testWFSCommandCreation() throws Exception {
        String jobId = UUID.randomUUID().toString();

        String params = "{\"translation\":\"MGCP.js\"," +
                         "\"inputtype\":\"db\"," +
                         "\"input\":\"ToyTestA\"," +
                         "\"outputtype\":\"wfs\"," +
                         "\"removereview\" : \"false\"}";

        ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
        ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());

        String actualCaller = exportCommand.get("caller").toString();
        String expectedCaller = this.getClass().getName();

        assertEquals(expectedCaller, actualCaller);

        String actualExectype = exportCommand.get("exectype").toString();
        String expectedExectype = "make";

        assertEquals(expectedExectype, actualExectype);

        String actualExec = exportCommand.get("exec").toString();
        String expectedExec = "osm2ogrscript";

        assertEquals(expectedExec, actualExec);

        String actualParams = exportCommand.get("params").toString();
        String expectedParams =
                "[{\"input\":\"ToyTestA\"}," +
                "{\"translation\":\"MGCP.js\"}," +
                "{\"inputtype\":\"db\"}," +
                "{\"outputtype\":\"wfs\"}," +
                "{\"removereview\":\"false\"}," +
                "{\"outputfolder\":\"" + TEMP_OUTPUT_PATH.replace("/", "\\/") + "\\/" + jobId + "\"}," +
                "{\"output\":\"" + jobId + "\"}," +
                "{\"DB_URL\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\"}," +
                "{\"OSM_API_DB_URL\":\"osmapidb:\\/\\/${OSMAPI_DB_USER}:${OSMAPI_DB_PASSWORD}@${OSMAPI_DB_HOST}:${OSMAPI_DB_PORT}\\/${OSMAPI_DB_NAME}\"}," +
                "{\"outputname\":\"" + jobId + "\"}," +
                "{\"PG_URL\":\"host='${HOOTAPI_DB_HOST}' port='${HOOTAPI_DB_PORT}' user='${HOOTAPI_DB_USER}' password='${HOOTAPI_DB_PASSWORD}' dbname='wfsstoredb'\"}]";

        assertEquals(expectedParams, actualParams);
    }

    @Test
    @Category(UnitTest.class)
    public void testExportToOsmApiDb() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);

            tags.put("osm_api_db_export_time", EXPORT_TIME);

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"db\"," +
                             "\"input\":\"" + MAP_NAME + "\"," +
                             "\"outputtype\":\"osm_api_db\"," +
                             "\"removereview\":\"false\"," +
                             "\"TASK_BBOX\":\"0.0,0.0,0.0,0.0\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());

            String actualCaller = exportCommand.get("caller").toString();
            String expectedCaller = this.getClass().getName();

            assertEquals(expectedCaller, actualCaller);

            String actualExectype = exportCommand.get("exectype").toString();
            String expectedExectype = "make";

            assertEquals(expectedExectype, actualExectype);

            String actualExec = exportCommand.get("exec").toString();
            String expectedExec = "osm2ogrscript";

            assertEquals(expectedExec, actualExec);

            String actualParams = exportCommand.get("params").toString();
            String expectedParams =
                    "[{\"input\":\"" + MAP_NAME + "\"}," +
                     "{\"inputtype\":\"db\"}," +
                     "{\"TASK_BBOX\":\"0.0,0.0,0.0,0.0\"}," +
                     "{\"outputtype\":\"osm_api_db\"}," +
                     "{\"removereview\":\"false\"}," +
                     "{\"outputfolder\":\"" + TEMP_OUTPUT_PATH.replace("/", "\\/") + "\\/" + jobId + "\"}," +
                     "{\"output\":\"" + jobId + "\"}," +
                     "{\"DB_URL\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\"}," +
                     "{\"OSM_API_DB_URL\":\"osmapidb:\\/\\/${OSMAPI_DB_USER}:${OSMAPI_DB_PASSWORD}@${OSMAPI_DB_HOST}:${OSMAPI_DB_PORT}\\/${OSMAPI_DB_NAME}\"}," +
                     "{\"changesetoutput\":\"" + TEMP_OUTPUT_PATH.replace("/", "\\/") + "\\/changeset-" + jobId + ".osc.sql\"}," +
                     "{\"writeStdOutToStatusDetail\":\"true\"}," +
                     "{\"changesetsourcedatatimestamp\":\"2016-05-04 10:15\"}," +
                     "{\"aoi\":\"0.0,0.0,0.0,0.0\"}]";

            assertEquals(expectedParams, actualParams);
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testExportToChangeset() throws Exception {
        String jobId = UUID.randomUUID().toString();

        String params = "{\"inputtype\":\"db\"," +
                         "\"input\":\"" + MAP_NAME + "\"," +
                         "\"outputtype\":\"osc\"," +
                         "\"USER_ID\":\"test_user\"," +
                         "\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}";

        ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
        ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());

        String actualCaller = exportCommand.get("caller").toString();
        String expectedCaller = this.getClass().getName();

        assertEquals(expectedCaller, actualCaller);

        String actualExectype = exportCommand.get("exectype").toString();
        String expectedExectype = "make";

        assertEquals(expectedExectype, actualExectype);

        String actualExec = exportCommand.get("exec").toString();
        String expectedExec = "osm2ogrscript";

        assertEquals(expectedExec, actualExec);

        String actualParams = exportCommand.get("params").toString();
        String expectedParams =
                "[{\"input\":\"" + MAP_NAME + "\"}," +
                 "{\"inputtype\":\"db\"}," +
                 "{\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}," +
                 "{\"USER_ID\":\"test_user\"}," +
                 "{\"outputtype\":\"osc\"}," +
                 "{\"outputfolder\":\"" + TEMP_OUTPUT_PATH.replace("/", "\\/") + "\\/" + jobId + "\"}," +
                 "{\"output\":\"" + jobId +"\"}," +
                 "{\"DB_URL\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\"}," +
                 "{\"OSM_API_DB_URL\":\"osmapidb:\\/\\/${OSMAPI_DB_USER}:${OSMAPI_DB_PASSWORD}@${OSMAPI_DB_HOST}:${OSMAPI_DB_PORT}\\/${OSMAPI_DB_NAME}\"}," +
                 "{\"input1\":\"osmapidb:\\/\\/${OSMAPI_DB_USER}:${OSMAPI_DB_PASSWORD}@${OSMAPI_DB_HOST}:${OSMAPI_DB_PORT}\\/${OSMAPI_DB_NAME}\"}," +
                 "{\"input2\":\"hootapidb:\\/\\/${HOOTAPI_DB_USER}:${HOOTAPI_DB_PASSWORD}@${HOOTAPI_DB_HOST}:${HOOTAPI_DB_PORT}\\/${HOOTAPI_DB_NAME}\\/MyTestMap\"}," +
                 "{\"aoi\":\"10.1,10.1,10.1,10.1\"}," +
                 "{\"userid\":\"test_user\"}]";

        assertEquals(expectedParams, actualParams);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbWithTranslation() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"db\"," +
                             "\"input\":\"" + MAP_NAME + "\"," +
                             "\"outputtype\":\"osm_api_db\"," +
                             "\"removereview\":\"false\"," +
                             "\"translation\":\"MyTranslation.js\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString()
                    .contains("Custom translation not allowed when exporting to OSM API database."));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }

    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbNoTimestampTag() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);

            tags.remove("osm_api_db_export_time");

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"db\"," +
                             "\"input\":\"" + MAP_NAME + "\"," +
                             "\"outputtype\":\"osm_api_db\"," +
                             "\"removereview\":\"false\"," +
                             "\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.CONFLICT.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("has no osm_api_db_export_time tag"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbMissingMap() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);

            tags.put("osm_api_db_export_time", EXPORT_TIME);
            BDDMockito.given(DbUtils.getMapIdByName(MAP_NAME)).willReturn(null);

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"db\"," +
                             "\"input\":\"" + MAP_NAME + "\"," +
                             "\"outputtype\":\"osm_api_db\"," +
                             "\"removereview\":\"false\"," +
                             "\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("No map exists with name"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportOsmApiDbNotEnabled() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);

            tags.put("osm_api_db_export_time", EXPORT_TIME);

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"db\"," +
                    "\"input\":\"" + MAP_NAME + "\"," +
                    "\"outputtype\":\"osm_api_db\"," +
                    "\"removereview\":\"false\"," +
                    "\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.INTERNAL_SERVER_ERROR.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString()
                    .contains("Attempted to export to an OSM API database but OSM API database support is disabled"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbBadInputType() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);

            tags.put("osm_api_db_export_time", EXPORT_TIME);

            String jobId = UUID.randomUUID().toString();

            String params = "{\"inputtype\":\"file\"," +
                    "\"input\":\"" + MAP_NAME + "\"," +
                    "\"outputtype\":\"osm_api_db\"," +
                    "\"removereview\":\"false\"," +
                    "\"TASK_BBOX\":\"10.1,10.1,10.1,10.1\"}";

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString()
                    .contains("When exporting to an OSM API database, the input type must be a Hootenanny API database."));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToChangesetNoTaskBbox() throws Exception {
        try {
            String params = "{\"inputtype\":\"db\"," +
                    "\"input\":\"" + MAP_NAME + "\"," +
                    "\"outputtype\":\"osc\"," +
                    "\"USER_ID\":\"test_user\"}";

            String jobId = UUID.randomUUID().toString();

            ExportCommandFactory exportCommandFactory = new ExportCommandFactory();
            ExportCommand exportCommand = exportCommandFactory.build(jobId, params, this.getClass());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("TASK_BBOX must be specified"));
            throw e;
        }
    }
}