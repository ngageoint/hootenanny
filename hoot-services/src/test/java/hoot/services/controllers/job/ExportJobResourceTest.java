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

import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.*;

import java.io.File;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sun.jersey.api.client.ClientResponse.Status;


public class ExportJobResourceTest {

    private static final Logger log = LoggerFactory.getLogger(ExportJobResourceTest.class);

    @Test
    @Category(UnitTest.class)
    public void testProcess() throws Exception {
        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"gdb\",\"removereview\" : \"false\"}";

        String tempOutputPath = HootProperties.getProperty("tempOutputPath");
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
        Response resp = spy.process(params);
        String result = resp.getEntity().toString();
        JSONParser parser = new JSONParser();
        JSONObject o = (JSONObject) parser.parse(result);
        String jobId = o.get("jobid").toString();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessForWFS() throws Exception {
        String wfsStoreDb = HootProperties.getProperty("wfsStoreDb");
        String userid = HootProperties.getProperty("dbUserId");
        String pwd = HootProperties.getProperty("dbPassword");
        String host = HootProperties.getProperty("dbHost");
        String[] hostParts = host.split(":");
        String pgUrl = "host='" + hostParts[0] + "' port='" + hostParts[1] + "' user='" + userid + "' password='" + pwd
                + "' dbname='" + wfsStoreDb + "'";

        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"wfs\",\"removereview\" : \"false\"}";

        String tempOutputPath = HootProperties.getProperty("tempOutputPath");
        JSONObject arg = new JSONObject();
        arg.put("outputfolder", tempOutputPath + "/" + "f9a8d471");
        tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},{\"outputtype\":\"wfs\"},"
                + "{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}," + "{\"PG_URL\":\"" + pgUrl
                + "\"}],\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.wfs.WfsManager\","
                + "\"method\":\"createWfsResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"f9a8d471\","
                + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection_sync\"}]";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((JobControllerBase) spy).postChainJobRquest(anyString(), anyString());
        Response resp = spy.process(params);
        String result = resp.getEntity().toString();
        JSONParser parser = new JSONParser();
        JSONObject o = (JSONObject) parser.parse(result);
        String jobId = o.get("jobid").toString();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportResources() throws Exception {
        ExportJobResource exRes = new ExportJobResource();
        Response resp = exRes.getExportResources();
        String result = resp.getEntity().toString();

        String homeFolder = HootProperties.getProperty("homeFolder");
        String translationExtPath = HootProperties.getProperty("translationExtPath");

        String transExtPath = homeFolder + "/" + "/plugins-local/script/utp";
        if (translationExtPath != null && translationExtPath.length() > 0) {
            transExtPath = translationExtPath;
        }

        String expected = "";
        File f = new File(transExtPath);
        if (f.exists() && f.isDirectory()) {
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
        final String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                .getPath()));

        ExportJobResource spy = Mockito.spy(new ExportJobResource());

        Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
        final List<Long> mapIds = new ArrayList<>();
        mapIds.add(new Long(1));
        Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
        java.util.Map<String, String> mapTags = new HashMap<>();
        final String exportTime = "2016-05-04 10:15";
        mapTags.put("osm_api_db_export_time", exportTime);
        Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
        final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
        final String mapBoundsStr = mapBounds.toServicesString();
        Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

        final String commandArgs = spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null)
                .toString();
        log.debug(commandArgs);

        Assert.assertTrue(commandArgs.contains("{\"input\":\"MyTestMap\"}"));
        Assert.assertTrue(commandArgs.contains("{\"outputtype\":\"osm_api_db\"}"));
        Assert.assertTrue(commandArgs.contains("{\"removereview\":\"false\"}"));
        Assert.assertTrue(commandArgs.contains("{\"inputtype\":\"db\"}"));
        Assert.assertTrue(commandArgs.contains("{\"temppath\":"));
        Assert.assertTrue(commandArgs.contains("{\"changesetsourcedatatimestamp\":\"" + exportTime + "\"}"));
        Assert.assertTrue(commandArgs.contains("{\"changesetaoi\":\"" + mapBoundsStr + "\"}"));
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbWithTranslation() throws Exception {
        try {
            final String inputParams = FileUtils
                    .readFileToString(new File(
                            Thread.currentThread()
                                    .getContextClassLoader()
                                    .getResource(
                                            "hoot/services/controllers/job/ExportResourceTestExportToOsmApiDbWithTranslationInput.json")
                                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            final List<Long> mapIds = new ArrayList<>();
            mapIds.add(new Long(1));
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
            java.util.Map<String, String> mapTags = new HashMap<>();
            final String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
            final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString()
                    .contains("Custom translation not allowed when exporting to OSM API database."));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbNoTimestampTag() throws Exception {
        try {
            final String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            final List<Long> mapIds = new ArrayList<>();
            mapIds.add(new Long(1));
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
            java.util.Map<String, String> mapTags = new HashMap<>();
            // add no map tags
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
            final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.CONFLICT.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("has no osm_api_db_export_time tag"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbBadInputType() throws Exception {
        try {
            final String inputParams = FileUtils.readFileToString(new File(Thread
                    .currentThread()
                    .getContextClassLoader()
                    .getResource(
                            "hoot/services/controllers/job/ExportResourceTestExportToOsmApiDbBadInputTypeInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            final List<Long> mapIds = new ArrayList<>();
            mapIds.add(new Long(1));
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
            java.util.Map<String, String> mapTags = new HashMap<>();
            final String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
            final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e
                    .getResponse()
                    .getEntity()
                    .toString()
                    .contains(
                            "When exporting to an OSM API database, the input type must be a Hootenanny API database."));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbMultipleMapsWithSameName() throws Exception {
        try {
            final String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            final List<Long> mapIds = new ArrayList<>();
            // add two map id's
            mapIds.add(new Long(1));
            mapIds.add(new Long(2));
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
            java.util.Map<String, String> mapTags = new HashMap<>();
            final String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
            final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("Multiple maps with name"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbMissingMap() throws Exception {
        try {
            final String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
            final List<Long> mapIds = new ArrayList<>();
            // add no map id's
            Mockito.doReturn(mapIds).when(spy).getMapIdsByName(anyString(), any(Connection.class));
            java.util.Map<String, String> mapTags = new HashMap<>();
            final String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong(), any(Connection.class));
            final BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), null).toString();
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().getEntity().toString().contains("No map exists with name"));
            throw e;
        }
    }
}