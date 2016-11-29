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

import static hoot.services.HootProperties.HOME_FOLDER;
import static org.junit.Assert.assertEquals;

import java.io.File;
import java.net.URL;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.ArgumentMatcher;
import org.mockito.Matchers;
import org.mockito.Mockito;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesSpringTestConfig;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
//@Transactional
public class JobResourceTest {
    private static final File homeFolder;
    private static final String original_HOME_FOLDER;

    static {
        try {
            original_HOME_FOLDER = HOME_FOLDER;
            homeFolder = new File(FileUtils.getTempDirectory(), "RasterToTilesResourceTest");
            FileUtils.forceMkdir(homeFolder);
            Assert.assertTrue(homeFolder.exists());
            HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());

            URL inputUrl = JobResourceTest.class.getResource("/hoot.services.controllers.job/services_fields_metadata.json");
            File dest = new File(new File(homeFolder, "scripts"), "services_fields_metadata.json");
            FileUtils.copyURLToFile(inputUrl, dest);
        }
        catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @BeforeClass
    public static void beforeClass() throws Exception {}

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
    }

    /**
     * Tests processChainJob in happy route
     *
     * @throws Exception
     */
    @Test
    @Category(UnitTest.class)
    public void testProcessChainJob() throws Exception {
        // Create Mock JobStatusManager Class
        JobStatusManager mockJobStatusManager = Mockito.mock(JobStatusManager.class);
        Mockito.doNothing().when(mockJobStatusManager).addJob(Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).updateJob(Matchers.anyString(), Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).setComplete(Matchers.anyString(), Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).setFailed(Matchers.anyString(), Matchers.anyString());

        // Mock child info
        JSONObject mockChild = new JSONObject();
        mockChild.put("id", "test_child_123");
        mockChild.put("detail", "processing");
        mockChild.put("status", "running");

        JobResource real = new JobResource();
        JobResource spy = Mockito.spy(real);

        Mockito.doReturn(Response.ok().build()).when(spy).processJob(Matchers.anyString(), Matchers.any(String.class));
        //Mockito.doReturn(mockJobStatusManager).when(spy).createJobStatusMananger();
        //Mockito.doReturn(mockChild).when(spy).execReflection(Matchers.anyString(),
        //                 Matchers.any(JSONObject.class), Matchers.any(JobStatusManager.class));

        String jobStr = "[{\"caller\":\"FileUploadResource\",\"exec\":\"makeetl\","
                + "\"params\":[{\"INPUT\":\"upload\\/81898818-2ca3-4a15-9421-50eb91952586\\/GroundPhotos.shp\"},"
                + "{\"INPUT_TYPE\":\"OGR\"},{\"TRANSLATION\":\"translations\\/UTP.js\"},{\"INPUT_NAME\":\"GroundPhotos\"}],"
                + "\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\","
                + "\"method\":\"ingestOSMResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"GroundPhotos\","
                + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";

        spy.processChainJob("test_job_id_1234", jobStr);

        // sleep for a couple of secs to make sure that all threads spawned by the call to spy.processChainJob() finish
        Thread.sleep(2000);

        class validParam2Matcher extends ArgumentMatcher<JSONObject> {

            @Override
            public boolean matches(Object argument) {
                JSONObject param = (JSONObject) argument;
                boolean val1 = param.get("class").toString()
                        .equals("hoot.services.controllers.ingest.RasterToTilesService");
                boolean val2 = param.get("method").toString().equals("ingestOSMResource");
                boolean val3 = param.get("exectype").toString().equals("reflection");

                JSONArray params = (JSONArray) param.get("params");
                JSONObject innerParam = (JSONObject) params.get(0);
                boolean val4 = innerParam.get("isprimitivetype").toString().equals("false");
                boolean val5 = innerParam.get("value").toString().equals("GroundPhotos");
                boolean val6 = innerParam.get("paramtype").toString().equals("java.lang.String");

                return val1 && val2 && val3 && val4 && val5 && val6;
            }
        }

        //Mockito.verify(spy).execReflection(Matchers.matches("test_job_id_1234"),
        //                                   Matchers.argThat(new validParam2Matcher()),
        //                                   Matchers.any(JobStatusManager.class));

        ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

        /*
        Mockito.verify(mockJobStatusManager, Mockito.times(3)).updateJob(Matchers.anyString(), argCaptor.capture());

        JSONParser parser = new JSONParser();

        List<String> args = argCaptor.getAllValues();
        JSONObject status = (JSONObject) parser.parse(args.get(0));
        JSONArray children = (JSONArray) status.get("children");
        JSONObject child = (JSONObject) children.get(0);

        assertEquals("processing", child.get("detail").toString());
        assertEquals("running", child.get("status").toString());

        status = (JSONObject) parser.parse(args.get(1));
        children = (JSONArray) status.get("children");
        assertEquals(1, children.size());

        child = (JSONObject) children.get(0);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());

        status = (JSONObject) parser.parse(args.get(2));
        children = (JSONArray) status.get("children");
        assertEquals(2, children.size());

        child = (JSONObject) children.get(0);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());

        child = (JSONObject) children.get(1);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());

        ArgumentCaptor<String> setCompleteArgCaptor = ArgumentCaptor.forClass(String.class);

        Mockito.verify(mockJobStatusManager,
                       Mockito.times(1)).setComplete(Matchers.anyString(), setCompleteArgCaptor.capture());

        args = setCompleteArgCaptor.getAllValues();
        status = (JSONObject) parser.parse(args.get(0));
        children = (JSONArray) status.get("children");

        assertEquals(2, children.size());

        child = (JSONObject) children.get(0);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());

        child = (JSONObject) children.get(1);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());
        */
    }

    /**
     * Tests the processChainJob failure by _execReflection
     *
     * @throws Exception
     */
    @Test
    @Category(UnitTest.class)
    public void testProcessChainJobFailure() throws Exception {
        // Create Mock JobStatusManager Class
        JobStatusManager mockJobStatusManager = Mockito.mock(JobStatusManager.class);
        Mockito.doNothing().when(mockJobStatusManager).addJob(Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).updateJob(Matchers.anyString(), Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).setComplete(Matchers.anyString(), Matchers.anyString());
        Mockito.doNothing().when(mockJobStatusManager).setFailed(Matchers.anyString(), Matchers.anyString());

        // Mock child info
        JSONObject mockChild = new JSONObject();
        mockChild.put("id", "test_child_123_FAIL");
        mockChild.put("detail", "processing");
        mockChild.put("status", "running");

        JobResource real = new JobResource();
        JobResource spy = Mockito.spy(real);

        // so I use this to avoid actual call
        Mockito.doReturn(Response.ok().build()).when(spy).processJob(Matchers.anyString(), Matchers.anyString());
        //Mockito.doReturn(mockJobStatusManager).when(spy).createJobStatusMananger();

        // failure point
        //Mockito.doThrow(new Exception("Mock failure for testing Process Chain Job failure. (Not real failure!!!)"))
        //       .when(spy).execReflection(Matchers.anyString(),
        //                                  Matchers.any(JSONObject.class),
        //                                  Matchers.any(JobStatusManager.class));

        try {
            String jobStr = "[{\"caller\":\"FileUploadResource\",\"exec\":\"makeetl\","
                    + "\"params\":[{\"INPUT\":\"upload\\/81898818-2ca3-4a15-9421-50eb91952586\\/GroundPhotos.shp\"},"
                    + "{\"INPUT_TYPE\":\"OGR\"},{\"TRANSLATION\":\"translations\\/UTP.js\"},{\"INPUT_NAME\":\"GroundPhotos\"}],"
                    + "\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\","
                    + "\"method\":\"ingestOSMResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"GroundPhotos\","
                    + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";

            spy.processChainJob("test_job_id_1234_FAIL", jobStr);

            // sleep for a couple of secs to make sure that all threads spawned by the the call to spy.processChainJob() finish
            Thread.sleep(2000);
        }
        catch (WebApplicationException wex) {
            Response res = wex.getResponse();
            assertEquals(500, res.getStatus());
        }

        // There should be one success for first part being completed. Second
        // would be setFailed which updates db directly so update is not called.
        ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

        /*
        Mockito.verify(mockJobStatusManager, Mockito.times(2)).updateJob(Matchers.anyString(), argCaptor.capture());

        JSONParser parser = new JSONParser();

        List<String> args = argCaptor.getAllValues();
        JSONObject status = (JSONObject) parser.parse(args.get(0));
        JSONArray children = (JSONArray) status.get("children");
        JSONObject child = (JSONObject) children.get(0);

        assertEquals("processing", child.get("detail").toString());
        assertEquals("running", child.get("status").toString());

        status = (JSONObject) parser.parse(args.get(1));
        children = (JSONArray) status.get("children");
        assertEquals(1, children.size());

        child = (JSONObject) children.get(0);
        assertEquals("success", child.get("detail").toString());
        assertEquals("complete", child.get("status").toString());

        // Check for setFailed invocation
        ArgumentCaptor<String> setFailedArgCaptor = ArgumentCaptor.forClass(String.class);

        Mockito.verify(mockJobStatusManager, Mockito.times(1)).setFailed(Matchers.anyString(), setFailedArgCaptor.capture());

        args = setFailedArgCaptor.getAllValues();
        status = (JSONObject) parser.parse(args.get(0));
        children = (JSONArray) status.get("children");

        assertEquals(1, children.size());

        child = (JSONObject) children.get(0);
        assertEquals("Mock failure for testing Process Chain Job failure. (Not real failure!!!)", child.get("detail").toString());
        assertEquals("failed", child.get("status").toString());
        */
    }
}
