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

import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.mockito.Matchers.anyString;

import java.io.File;
import java.io.IOException;
import java.io.StringWriter;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import static org.junit.Assert.*;

import org.apache.commons.io.FileUtils;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.UnitTest;


public class DeriveChangesetResourceTest {

    private static final Logger logger = LoggerFactory.getLogger(DeriveChangesetResourceTest.class);
    
    @Test
    @Category(UnitTest.class)
    public void testDeriveChangeset() {
        DeriveChangesetResource spy = Mockito.spy(new DeriveChangesetResource());
        
        Mockito.doNothing().when((JobControllerBase) spy).postJobRequest(anyString(), anyString());
        String jobId = spy.derive("input1.osm", "input2.osm", "-77.04,38.8916,-77.03324,38.8958").getJobid();
        
        String paramsEnd = "\"input2\":\"input2.osm\",\"input1\":\"input1.osm\",\"aoi\":\"-77.04,38.8916,-77.03324,38.8958\"}],\"exectype\":\"make\",\"exec\":\"derivechangeset\"}";
        //we're just checking the params sent into the job launcher here
        //not sure how to do this in one call...so here's two
        Mockito.verify(spy).postJobRequest(Matchers.matches(jobId), Matchers.endsWith(paramsEnd));
        Mockito.verify(spy).postJobRequest(Matchers.matches(jobId), Matchers.contains("\"output\":"));
    }
    
    @Test
    @Category(UnitTest.class)
    public void testGetDerivedChangeset() throws Exception {
        //read a dummy changeset file
        String changesetText = FileUtils.readFileToString(new File(Thread.currentThread()
                .getContextClassLoader()
                .getResource("hoot.services.controllers.job/DeriveChangesetResourceTestInput.osc")
                .getPath()), 
                "UTF-8");
        
        //write the contents of the dummy file to a new temp file
        File tempOutputDir = new File(TEMP_OUTPUT_PATH);
        if (!tempOutputDir.exists())
        {
          tempOutputDir.mkdir();   
        }
        File changesetFile = File.createTempFile("changeset", ".osc", tempOutputDir);
        changesetFile.deleteOnExit();
        FileUtils.writeStringToFile(changesetFile, changesetText, "UTF-8");
        
        //force the get method to return the path to the temp file
        String changesetFilePath = changesetFile.getAbsolutePath();
        DeriveChangesetResource spy = Mockito.spy(new DeriveChangesetResource());
        Mockito.doReturn(changesetFilePath).when(spy).getJobStatusDetail(anyString());
        
        //make sure the same xml comes back in the method response
        Response response = spy.get(anyString());
        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        DOMSource test = (DOMSource)response.getEntity();
        Transformer transformer = transformerFactory.newTransformer();
        String changesetXml;
        try (StringWriter writer = new StringWriter()) {
            transformer.transform(test, new StreamResult(writer));
            changesetXml = writer.getBuffer().toString();
        } 
        assertEquals(changesetText, changesetXml);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetDerivedChangesetInvalidJobId() throws WebApplicationException {
        try {
            //try to retrieve a changeset file with a non-existent changeset id; should fail
            (new DeriveChangesetResource()).get("blah");
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString()
                    .contains("Error fetching changeset contents for job ID=blah.  Unable to read changeset temp file at"));
            throw e;
        }
    }
    
    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetDerivedChangesetTempFileNoLongerExists() throws Exception {
        try {
            //force the job id to exist but then try to access a non-existent changeset file;
            //should fail
            DeriveChangesetResource spy = Mockito.spy(new DeriveChangesetResource());
            Mockito.doReturn("blah").when(spy).getJobStatusDetail(anyString());
            spy.get(anyString());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("Unable to read changeset temp file at"));
            throw e;
        }
    }
    
    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetDerivedChangesetMultipleFiles() throws Exception {
        try {
            //force the job id to exist and force a multiple changeset files situation; should fail
            //we don't currently handle this situation; see hoot-core DeriveChangesetCmd
            DeriveChangesetResource spy = Mockito.spy(new DeriveChangesetResource());
            Mockito.doReturn("<multiple files>").when(spy).getJobStatusDetail(anyString());
            spy.get(anyString());
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.INTERNAL_SERVER_ERROR.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("Changeset requested is made up of multiple changesets in multiple files.  /getderived does not currently support this."));
            throw e;
        }
    }
}