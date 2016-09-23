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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import static hoot.services.HootProperties.*;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.anyLong;
import static org.mockito.Mockito.verify;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.utils.HootCustomPropertiesSetter;


public class ExportJobResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testProcess() throws Exception {
        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"gdb\",\"removereview\" : \"false\"}";

        String tempOutputPath = TEMP_OUTPUT_PATH;
        JSONObject arg = new JSONObject();
        arg.put("outputfolder", tempOutputPath + "/" + "f9a8d471");
        tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},";
        jobArgs += "{\"outputtype\":\"gdb\"},{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}],";
        jobArgs += "\"exectype\":\"make\"}";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
        JobId resp = spy.process(params);
        String jobId = resp.getJobid();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessForWFS() throws Exception {
        String[] hostParts = DB_HOST.split(":");
        String pgUrl = "host='" + hostParts[0] + "' port='" + hostParts[1] + "' user='" + DB_USER_ID + "' password='" + DB_PASSWORD
                + "' dbname='" + WFS_STORE_DB + "'";

        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"wfs\",\"removereview\" : \"false\"}";

        JSONObject arg = new JSONObject();
        arg.put("outputfolder", TEMP_OUTPUT_PATH + "/" + "f9a8d471");
        String tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},{\"outputtype\":\"wfs\"},"
                + "{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}," + "{\"PG_URL\":\"" + pgUrl
                + "\"}],\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.wfs.WfsManager\","
                + "\"method\":\"createWfsResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"f9a8d471\","
                + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection_sync\"}]";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((JobControllerBase) spy).postChainJobRquest(anyString(), anyString());
        JobId resp = spy.process(params);
        String jobId = resp.getJobid();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportResources() throws Exception {
        ExportJobResource exRes = new ExportJobResource();
        Response resp = exRes.getExportResources();
        String result = resp.getEntity().toString();

        String transExtPath = HOME_FOLDER + "/" + "/plugins-local/script/utp";
        if (!TRANSLATION_EXT_PATH.isEmpty()) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        String expected = "";
        File file = new File(transExtPath);
        if (file.exists() && file.isDirectory()) {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"},{\"description\":\"UTP\",\"name\":\"UTP\"}]";
        }
        else {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"}]";
        }
        Assert.assertEquals(expected, result);
    }

    @Test
    @Category(UnitTest.class)
    public void testExportToOsmApiDb() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());
            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(new Long(1));
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            String mapBoundsStr = mapBounds.toServicesString();
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            String commandArgs = spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();

            Assert.assertTrue(commandArgs.contains("{\"input\":\"MyTestMap\"}"));
            Assert.assertTrue(commandArgs.contains("{\"outputtype\":\"osm_api_db\"}"));
            Assert.assertTrue(commandArgs.contains("{\"removereview\":\"false\"}"));
            Assert.assertTrue(commandArgs.contains("{\"inputtype\":\"db\"}"));
            Assert.assertTrue(commandArgs.contains("{\"temppath\":"));
            Assert.assertTrue(commandArgs.contains("{\"changesetsourcedatatimestamp\":\"" + exportTime + "\"}"));
            Assert.assertTrue(commandArgs.contains("{\"changesetaoi\":\"" + mapBoundsStr + "\"}"));
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbWithTranslation() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils
                    .readFileToString(new File(
                            Thread.currentThread()
                                    .getContextClassLoader()
                                    .getResource("hoot/services/controllers/job/ExportResourceTestExportToOsmApiDbWithTranslationInput.json")
                                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(1L);
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString()
                    .contains("Custom translation not allowed when exporting to OSM API database."));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbNoTimestampTag() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(1L);
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            // add no map tags
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.CONFLICT.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("has no osm_api_db_export_time tag"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbBadInputType() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread
                    .currentThread()
                    .getContextClassLoader()
                    .getResource(
                            "hoot/services/controllers/job/ExportResourceTestExportToOsmApiDbBadInputTypeInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(1L);
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e
                    .getResponse()
                    .getEntity()
                    .toString()
                    .contains("When exporting to an OSM API database, the input type must be a Hootenanny API database."));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbMultipleMapsWithSameName() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            // add two map id's
            mapIds.add(1L);
            mapIds.add(2L);
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("Multiple maps with name"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbMissingMap() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            // add no map id's
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("No map exists with name"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportOsmApiDbNotEnabled() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(1L);

            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();

            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());

            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Response.Status.INTERNAL_SERVER_ERROR.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString()
                    .contains("Attempted to export to an OSM API database but OSM API database support is disabled"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
        }
    }
}