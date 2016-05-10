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
package hoot.services.controllers.info;

import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Properties;

import javax.ws.rs.core.MediaType;

import org.apache.commons.lang3.StringUtils;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.test.framework.JerseyTest;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.info.BuildInfo;
import hoot.services.info.CoreDetail;
import hoot.services.info.ServicesDetail;
import hoot.services.info.VersionInfo;

//Use PowerMock here instead of Mockito, so we can mock a static method.
@RunWith(PowerMockRunner.class)
@PrepareForTest({BuildInfo.class})
public class AboutResourceTest extends JerseyTest {

    public AboutResourceTest() {
        super("hoot.services.controllers.info");
    }

    private static void mockBuildInfo() throws IOException {
        //mock the existence of the build info properties
        Properties buildInfoProps = new Properties();
        buildInfoProps.setProperty("name", "Hootenanny Web Services");
        buildInfoProps.setProperty("version", "0.0.1");
        buildInfoProps.setProperty("user", "testuser");
        PowerMockito.mockStatic(BuildInfo.class);
        PowerMockito.when(BuildInfo.getInstance()).thenReturn(buildInfoProps);
    }

    @Test
    @Category(UnitTest.class)
    public void getServicesVersionInfo() throws IOException {
        mockBuildInfo();

        VersionInfo responseData = null;
        try {
            responseData =
                    resource()
                            .path("/about/servicesVersionInfo")
                            .accept(MediaType.APPLICATION_JSON)
                            .get(VersionInfo.class);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }

        Assert.assertEquals("Hootenanny Web Services", responseData.getName());
        Assert.assertEquals("0.0.1", responseData.getVersion());
        Assert.assertEquals("testuser", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void getServicesVersionDetail() throws IOException {
        Properties hootProps = HootProperties.getInstance();
        hootProps.clear();
        hootProps.setProperty("testProp1", "testVal1");
        HootProperties.setProperties(hootProps);

        mockBuildInfo();

        ServicesDetail responseData = null;
        try {
            responseData =
                    resource()
                            .path("/about/servicesVersionDetail")
                            .accept(MediaType.APPLICATION_JSON)
                            .get(ServicesDetail.class);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }

        Assert.assertNotNull(StringUtils.trimToNull(responseData.getClassPath()));
        Assert.assertEquals(1, responseData.getProperties().length);
        Assert.assertEquals("testProp1", responseData.getProperties()[0].getName());
        Assert.assertEquals("testVal1", responseData.getProperties()[0].getValue());
        //not sure of a better way to test this one yet...
        Assert.assertTrue(responseData.getResources().length > 0);
    }

    @Test
    @Category(UnitTest.class)
    public void getCoreVersionInfo() throws IOException {
        mockBuildInfo();

        VersionInfo responseData = null;
        try {
            responseData =
                    resource()
                            .path("/about/coreVersionInfo")
                            .accept(MediaType.APPLICATION_JSON)
                            .get(VersionInfo.class);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }

        Assert.assertEquals("Hootenanny Core", responseData.getName());
        Assert.assertEquals("0.2.23_1036_ga13f8a9_dirty", responseData.getVersion());
        Assert.assertNotNull("vagrant", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void getCoreVersionDetail() throws IOException {
        mockBuildInfo();

        CoreDetail responseData = null;
        try {
            responseData =
                    resource()
                            .path("/about/coreVersionDetail")
                            .accept(MediaType.APPLICATION_JSON)
                            .get(CoreDetail.class);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }

        //not a great way to test this, but haven't thought of anything better yet
        String firstEnvInfo = responseData.getEnvironmentInfo()[0];
        Assert.assertFalse(firstEnvInfo.contains("Built"));
        Assert.assertTrue(firstEnvInfo.contains("VersionCmd"));
    }

    @Test
    @Category(UnitTest.class)
    public void getServicesVersionNoBuildFile() throws Exception {
        //Use PowerMock here so we can mock a static method.
        PowerMockito.mockStatic(BuildInfo.class);

        //simulate the build.info file not existing
        PowerMockito.when(BuildInfo.getInstance()).thenThrow(IOException.class);

        VersionInfo responseData = null;
        try {
            responseData =
                    resource()
                            .path("/about/servicesVersionInfo")
                            .accept(MediaType.APPLICATION_JSON)
                            .get(VersionInfo.class);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }

        Assert.assertEquals("unknown", responseData.getName());
        Assert.assertEquals("unknown", responseData.getVersion());
        Assert.assertEquals("unknown", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void testCoreVersionParser() throws Exception {
        AboutResource aboutResource = new AboutResource();
        Method privateMethod = AboutResource.class.getDeclaredMethod("parseCoreVersionOutOf", String.class, Boolean.TYPE);
        privateMethod.setAccessible(true);

        String returnValue = (String) privateMethod.invoke(aboutResource,
                "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", false);
        Assert.assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "This is just a line\nHootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", true);
        Assert.assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "14:27:08.974 WARN ...rc/main/cpp/hoot/core/Hoot.cpp( 135) \n " +
                "Cannot load library HootHadoop: (libhdfs.so.0: cannot open shared object file: No such file or directory)\n" +
                "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", true);
        Assert.assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "14:27:08.974 WARN ...rc/main/cpp/hoot/core/Hoot.cpp( 135) \n " +
                "Cannot load library HootHadoop: (libhdfs.so.0: cannot open shared object file: No such file or directory)",
                true);
        Assert.assertEquals("Unable to determine!", returnValue);

    }
}
