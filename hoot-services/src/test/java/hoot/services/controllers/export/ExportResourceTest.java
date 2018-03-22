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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;
import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.*;

import java.io.File;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.commons.io.FileUtils;
import org.json.simple.parser.JSONParser;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.common.ZIPDirectoryContentsCommand;
import hoot.services.command.common.ZIPFileCommand;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.Job;
import hoot.services.utils.DbUtils;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ExportResourceTest extends HootServicesJerseyTestAbstract {

    private static String originalTEMP_OUTPUT_PATH;

    @Autowired
    private ExternalCommandManager externalCommandInterface;

    @BeforeClass
    public static void beforeClass() throws Exception {
        originalTEMP_OUTPUT_PATH = TEMP_OUTPUT_PATH;
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", "/tmp");
    }

    @AfterClass
    public static void afterClass() throws Exception {
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", originalTEMP_OUTPUT_PATH);
    }

    @Test
    @Category(UnitTest.class)
    public void testExportOSMResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("osm");
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setInputType("file");

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertSame(ExportOSMCommand.class, job.getCommands()[0].getClass());
        assertSame(ZIPFileCommand.class, job.getCommands()[1].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportOSMPBFResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("osm.pbf");
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setInputType("file");

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(1, job.getCommands().length);
        assertSame(ExportOSMCommand.class, job.getCommands()[0].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportOSCResource() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("osc");
        exportParams.setInput(Long.toString(mapId));
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setBounds(aoi);
        exportParams.setUserId(String.valueOf(userId));

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(1, job.getCommands().length);
        assertSame(DeriveChangesetCommand.class, job.getCommands()[0].getClass());
    }
    
    @Test
    @Category(UnitTest.class)
    public void testExportTilesResource() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams jobParams = new ExportParams();
        jobParams.setOutputName("output");
        jobParams.setAppend(false);
        jobParams.setTextStatus(false);
        jobParams.setInputType("file");
        jobParams.setOutputType("tiles");
        jobParams.setInput("input1;input2");
        jobParams.setBounds(aoi);
        jobParams.setMaxNodeCountPerTile(1000);
        jobParams.setPixelSize(0.001);
        jobParams.setUserId(String.valueOf(userId));

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(jobParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(1, job.getCommands().length);
        assertSame(CalculateTilesCommand.class, job.getCommands()[0].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportOSMAPIDBResource() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("osm_api_db");
        exportParams.setInput(Long.toString(mapId));
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setBounds(aoi);
        exportParams.setUserId(String.valueOf(userId));

        String conflictTimestamp = "2017-04-18 14:00:01.234";
        Map<String, String> tags = new HashMap<>();
        tags.put("osm_api_db_export_time", conflictTimestamp);
        DbUtils.updateMapsTableTags(tags, mapId);

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertSame(DeriveChangesetCommand.class, job.getCommands()[0].getClass());
        assertSame(ApplyChangesetCommand.class, job.getCommands()[1].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportShapeResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("shp");
        exportParams.setTranslation("translations/TDSv40.js");
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertSame(ExportCommand.class, job.getCommands()[0].getClass());
        assertSame(ZIPDirectoryContentsCommand.class, job.getCommands()[1].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportFGDBResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("gdb");
        exportParams.setTranslation("translations/TDSv40.js");
        exportParams.setInput("input");
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");

        String responseData = target("export/execute")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(exportParams, MediaType.APPLICATION_JSON), String.class);

        assertNotNull(responseData);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertSame(ExportCommand.class, job.getCommands()[0].getClass());
        assertSame(ZIPDirectoryContentsCommand.class, job.getCommands()[1].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportResources() throws Exception {
        ExportResource exRes = new ExportResource();
        Response resp = exRes.getExportResources();
        String result = resp.getEntity().toString();

        String transExtPath = HOME_FOLDER + "/" + "/plugins-local/script/utp";
        if (!TRANSLATION_EXT_PATH.isEmpty()) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        String expected = "";
        File file = new File(transExtPath);
        if (file.exists() && file.isDirectory()) {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"}," +
                        "{\"description\":\"MGCP\",\"name\":\"MGCP\"}," +
                        "{\"description\":\"UTP\",\"name\":\"UTP\"}]";
        }
        else {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"}]";
        }

        JSONParser parser = new JSONParser();
        assertEquals(parser.parse(expected), parser.parse(result));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportedXmlFile() throws Exception {
        // read a dummy changeset file
        String changesetText = FileUtils.readFileToString(new File(Thread.currentThread().getContextClassLoader()
                .getResource("hoot/services/controllers/export/ExportJobResourceTestChangesetInput.osc").getPath()),
                "UTF-8");

        // write the contents of the dummy file to a new output file; mimic how
        // ExportJobResource::process would write it
        String jobId = UUID.randomUUID().toString();
        File changesetFile = new File(HootProperties.TEMP_OUTPUT_PATH + "/" + jobId + "/" + jobId + ".osc");
        changesetFile.deleteOnExit(); //remove this if removeCache is enabled
        FileUtils.writeStringToFile(changesetFile, changesetText, "UTF-8");

        ExportResource spy = Mockito.spy(new ExportResource());
        Response response = spy.getXmlOutput(jobId,  "osc");
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
            // id; should fail with a 404
            (new ExportResource()).getXmlOutput("blah", "osc");
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
                    .getResource("hoot/services/controllers/export/ExportJobResourceTestChangesetInput.osc").getPath()),
                    "UTF-8");

            // write the contents of the dummy file to multiple new output
            // files; mimic how ExportJobResource::process would write it and how changeset-derive would handle a
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

            ExportResource spy = Mockito.spy(new ExportResource());
            /* Response response = */spy.getXmlOutput(jobId, "osc");
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