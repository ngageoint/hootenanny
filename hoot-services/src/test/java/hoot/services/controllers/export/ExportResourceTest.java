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

import static hoot.services.HootProperties.*;
import static org.junit.Assert.*;

import java.io.File;
import java.io.StringWriter;
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
import org.w3c.dom.Document;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.UnitTest;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.XmlDocumentBuilder;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ExportResourceTest {

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
            hootCommand.put("input1", HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/AllDataTypesA.osm");
            hootCommand.put("input2", HOME_FOLDER + "/hoot-services/src/test/resources/hoot/services/controllers/export/ExportJobResourceTestAdtConflated.osm");
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
            // id; should fail with a 404
            (new ExportResource()).getXmlOutput("blah", true, "osc");
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

            ExportResource spy = Mockito.spy(new ExportResource());
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