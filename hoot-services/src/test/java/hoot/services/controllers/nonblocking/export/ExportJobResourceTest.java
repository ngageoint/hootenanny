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
package hoot.services.controllers.nonblocking.export;

import static hoot.services.HootProperties.*;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.anyLong;

import java.io.File;
import java.io.IOException;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.UUID;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.commons.io.FileUtils;
import org.apache.xpath.XPathAPI;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.UnitTest;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.controllers.nonblocking.AsynchronousJobResource;
import hoot.services.controllers.nonblocking.JobId;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesSpringTestConfig;
import hoot.services.utils.JsonUtils;
import hoot.services.utils.XmlDocumentBuilder;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@ActiveProfiles("test")
@Transactional
public class ExportJobResourceTest {

    @Autowired
    private ExternalCommandManager externalCommandInterface;

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
        Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
        JobId resp = spy.process(params);
        String jobId = resp.getJobid();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        // verify(spy).postJobRequest(Matchers.matches(jobId),
        // Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessForWFS() throws Exception {
        String pgUrl = replaceSensitiveData("host='" + DB_HOST + "' port='" + DB_PORT + "' user='" + DB_USER_ID + "' password='" + DB_PASSWORD
                + "' dbname='" + WFS_STORE_DB + "'");

        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"wfs\",\"removereview\" : \"false\"}";

        JSONObject arg = new JSONObject();
        arg.put("outputfolder", TEMP_OUTPUT_PATH + "/" + "f9a8d471");
        String tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},{\"outputtype\":\"wfs\"},"
                + "{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}," + "{\"PG_URL\":\"" + pgUrl
                + "\"}],\"exectype\":\"make\"},{\"class\":\"hoot.services.wfs.WfsManager\","
                + "\"method\":\"createWfsResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"f9a8d471\","
                + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection_sync\"}]";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((AsynchronousJobResource) spy).processChainJob(any());
        JobId resp = spy.process(params);
        String jobId = resp.getJobid();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        // verify(spy).postChainJobRequest(Matchers.matches(jobId),
        // Matchers.endsWith(jobArgs));
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
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},"
                    + "{\"description\":\"MGCP\",\"name\":\"MGCP\"}," + "{\"description\":\"UTP\",\"name\":\"UTP\"}]";
        }
        else {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"}]";
        }

        JSONParser parser = new JSONParser();
        assertEquals(parser.parse(expected), parser.parse(result));
    }

    @Test
    @Category(UnitTest.class)
    public void testExportToOsmApiDb() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());
            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            String mapBoundsStr = mapBounds.toServicesString();
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            String commandArgs = spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams),
                    (JSONObject) new JSONParser().parse(inputParams)).toString();

            assertTrue(commandArgs.contains("{\"input\":\"MyTestMap\"}"));
            assertTrue(commandArgs.contains("{\"outputtype\":\"osm_api_db\"}"));
            assertTrue(commandArgs.contains("{\"removereview\":\"false\"}"));
            assertTrue(commandArgs.contains("{\"inputtype\":\"db\"}"));
            assertTrue(commandArgs.contains("{\"changesetoutput\":"));
            assertTrue(commandArgs.contains("{\"changesetsourcedatatimestamp\":\"" + exportTime + "\"}"));
            assertTrue(commandArgs.contains("{\"aoi\":\"" + mapBoundsStr + "\"}"));
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbWithTranslation() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils
                    .readFileToString(
                            new File(
                                    Thread.currentThread()
                                            .getContextClassLoader()
                                            .getResource(
                                                    "hoot/services/controllers/nonblocking/export/ExportResourceTestExportToOsmApiDbWithTranslationInput.json")
                                            .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams));
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
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            // add no map tags
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams));
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
    public void testExportToOsmApiDbBadInputType() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils
                    .readFileToString(
                            new File(
                                    Thread.currentThread()
                                            .getContextClassLoader()
                                            .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestExportToOsmApiDbBadInputTypeInput.json")
                                            .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams));
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e
                    .getResponse()
                    .getEntity()
                    .toString()
                    .contains("When exporting to an OSM API database, the input type must be a Hootenanny API database."));
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
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = null;
            // add no map id's
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams));
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
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());
            Long mapId = 1L;

            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();

            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());

            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(JsonUtils.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams));
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

    @Test
    @Category(UnitTest.class)
    public void testExportToChangeset() throws IOException, ParseException {
        String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessChangesetInputInput.json")
                .getPath()), "UTF-8");

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());

        String commandArgs = spy.getExportToChangesetCommandArgs(JsonUtils.parseParams(inputParams),
                (JSONObject) new JSONParser().parse(inputParams)).toString();

        // outputfolder and output name are dependent upon the created job id,
        // which is only controllable
        // if you mock postJobRequest. That's already being done in testProcess,
        // so won't redo it here.
        assertTrue(commandArgs.contains("{\"input1\":\"" + OSM_APIDB_URL.replace("/", "\\/") + "\"}"));
        assertTrue(commandArgs.contains("{\"input2\":\"" + HOOT_APIDB_URL.replace("/", "\\/") + "\\/MyTestMap\"}"));
        assertTrue(commandArgs.contains("{\"outputtype\":\"osc\"}"));
        assertTrue(commandArgs.contains("{\"inputtype\":\"db\"}"));
        assertTrue(commandArgs.contains("{\"aoi\":\"0.0,0.0,0.0,0.0\"}"));
    }
    
    /*
     * This tests the export make script that the changeset derivation export uses.  This technically doesn't
     * test ExportJobResource at all but will leave it in this test class for lack of a better
     * place to put it.
     */
    @Test
    @Category(IntegrationTest.class)
    public void testExportToChangesetScript() throws Exception {
        File outputFile = null;
        try {
            //mock the command sent by DeriveChangesetResource to ProcessStreamInterface
            JSONArray commandArgs = new JSONArray();
            JSONObject hootCommand = new JSONObject();
            ExternalCommand command = new ExternalCommand();
            
            String jobId = UUID.randomUUID().toString();
            hootCommand.put("jobid", jobId);
            //don't want an integration with the database here, so just testing file inputs instead
            hootCommand.put("input1", HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/nonblocking/export/AllDataTypesA.osm");
            hootCommand.put("input2", HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/nonblocking/export/ExportJobResourceTestAdtConflated.osm");
            hootCommand.put("aoi", "-104.8192,38.8162,-104.6926,38.9181");
            //mimic the way ExportJobResource sets up the output path
            hootCommand.put("outputfolder", TEMP_OUTPUT_PATH + "/" + jobId);
            hootCommand.put("outputname", jobId);
            hootCommand.put("outputtype", "osc");
            outputFile = new File(hootCommand.get("outputfolder") + "/" + hootCommand.get("outputname") + "." + hootCommand.get("outputtype"));
            commandArgs.add (hootCommand);
            
            command.put("exectype", "make");
            command.put("exec", EXPORT_SCRIPT);
            command.put("caller", this.getClass().getSimpleName());
            command.put("params", commandArgs);

            this.externalCommandInterface.exec(null, command);
            
            //verify output file - we're not going to do an exact diff on it to avoid a dependency on 
            //core changeset generation logic which may change in the future.  just going to check for 
            //non-zero element counts on create/delete, which is good enough indication that the 
            //changeset generated without a failure
            Document actualChangesetDoc = XmlDocumentBuilder.parse(FileUtils.readFileToString(outputFile, "UTF-8"));
            assert(XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange").getLength() == 1);
            assert(XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange/create").getLength() > 0);
            assert(XPathAPI.selectNodeList(actualChangesetDoc, "//osmChange/delete").getLength() > 0);
        }
        finally {
            FileUtils.deleteQuietly(outputFile);
            if (outputFile != null) {
                assertTrue(!outputFile.exists());
            }
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToChangesetNoTaskBbox() throws Exception {
        try {
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportResourceTestProcessChangesetInputInput.json")
                    .getPath()), "UTF-8");
            inputParams = inputParams.replace("\"TASK_BBOX\":\"0.0,0.0,0.0,0.0\"", "");
            inputParams = inputParams.replace("osc\",", "osc\"");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());
            Mockito.doNothing().when((AsynchronousJobResource) spy).processJob(any());

            /* String commandArgs = */spy.getExportToChangesetCommandArgs(JsonUtils.parseParams(inputParams),
                    (JSONObject) new JSONParser().parse(inputParams));

        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("TASK_BBOX must be specified"));
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportedXmlFile() throws Exception {
        // read a dummy changeset file
        String changesetText = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                .getResource("hoot/services/controllers/nonblocking/export/ExportJobResourceTestChangesetInput.osc").getPath()),
                "UTF-8");

        // write the contents of the dummy file to a new output file; mimic how
        // ExportJobResource::process would write it
        String jobId = UUID.randomUUID().toString();
        File changesetFile = new File(HootProperties.TEMP_OUTPUT_PATH + "/" + jobId + "/" + jobId + ".osc");
        changesetFile.deleteOnExit(); //remove this if removeCache is enabled
        FileUtils.writeStringToFile(changesetFile, changesetText, "UTF-8");

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Response response = spy.getXmlOutput(jobId, true, "osc");
        DOMSource test = (DOMSource) response.getEntity();

        // parse out the returned xml and verify its what was originally written
        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        Transformer transformer = transformerFactory.newTransformer();
        String changesetXml;
        try (StringWriter writer = new StringWriter()) {
            transformer.transform(test, new StreamResult(writer));
            changesetXml = writer.getBuffer().toString();
        }

        assertEquals(changesetText, changesetXml);
        //assertTrue(!changesetFile.exists()); //enable this if removeCache is enabled
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetXmlOutputInvalidJobId() throws Exception {
        try {
            // try to retrieve a changeset file with a non-existent changeset
            // id; should fail with a
            // 404
            (new ExportJobResource()).getXmlOutput("blah", true, "osc");
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("Missing output file"));
            throw e;
        }
    }

    // Choosing not to handle changesets here that go over the max allowed size for now, as they will be
    // stored as separate changesets in multiple files.  The logic for it could be added in the future, 
    // if necessary.  Regardless, a more specific error message could still be desired here right now anyway.
    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetXmlOutputMultipleFiles() throws Exception {
        File changesetDir = null;
        try {
            // read a dummy changeset file
            String changesetText = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/export/ExportJobResourceTestChangesetInput.osc").getPath()),
                    "UTF-8");

            // write the contents of the dummy file to multiple new output
            // files; mimic how ExportJobResource::process would write it and how derive-changeset would handle a
            // changeset larger than the max allowable size (move multiple changeset files to their
            // own new folder; see OsmChangesetXmlWriter)
            String jobId = UUID.randomUUID().toString();
            changesetDir = new File(HootProperties.TEMP_OUTPUT_PATH + "/" + jobId + "/" + jobId);
            File changesetFile1 = new File(changesetDir + "/" + jobId + "-001.osc");
            changesetFile1.deleteOnExit();
            FileUtils.writeStringToFile(changesetFile1, changesetText, "UTF-8");
            File changesetFile2 = new File(changesetDir + "/" + jobId + "-002.osc");
            changesetFile2.deleteOnExit();
            FileUtils.writeStringToFile(changesetFile2, changesetText, "UTF-8");

            ExportJobResource spy = Mockito.spy(new ExportJobResource());
            /* Response response = */spy.getXmlOutput(jobId, true, "osc");
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("Missing output file"));
            throw e;
        }
        finally {
            FileUtils.deleteQuietly(changesetDir);
            assertTrue(!changesetDir.exists());
        }
    }
}