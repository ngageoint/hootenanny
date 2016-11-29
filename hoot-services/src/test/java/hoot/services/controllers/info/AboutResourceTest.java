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

import static org.junit.Assert.*;

import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Properties;

import javax.ws.rs.core.MediaType;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootServicesJerseyTestAbstract;


@RunWith(PowerMockRunner.class)
@PrepareForTest({ BuildInfo.class })
public class AboutResourceTest extends HootServicesJerseyTestAbstract {

    public AboutResourceTest() {}

    private static void mockBuildInfo() {
        // mock the existence of the build info properties
        Properties buildInfoProps = new Properties();
        buildInfoProps.setProperty("name", "Hootenanny Web Services");
        buildInfoProps.setProperty("version", "0.0.1");
        buildInfoProps.setProperty("user", "testuser");
        PowerMockito.mockStatic(BuildInfo.class);
        PowerMockito.when(BuildInfo.getInfo()).thenReturn(buildInfoProps);
    }

    @Test
    @Category(UnitTest.class)
    public void getServicesVersionInfo() throws IOException {
        mockBuildInfo();

        VersionInfo responseData =
                target("/about/servicesVersionInfo").request(MediaType.APPLICATION_JSON).get(VersionInfo.class);

        assertEquals("Hootenanny Web Services", responseData.getName());
        assertEquals("0.0.1", responseData.getVersion());
        assertEquals("testuser", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void getCoreVersionInfo() throws Exception {
        mockBuildInfo();

        VersionInfo responseData =
                target("/about/coreVersionInfo").request(MediaType.APPLICATION_JSON).get(VersionInfo.class);

        assertEquals("Hootenanny Core", responseData.getName());
        assertEquals("0.2.23_1036_ga13f8a9_dirty", responseData.getVersion());
        assertNotNull("vagrant", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void getCoreVersionDetail() throws IOException {
        mockBuildInfo();

        CoreDetail responseData =
                target("/about/coreVersionDetail").request(MediaType.APPLICATION_JSON).get(CoreDetail.class);

        // not a great way to test this, but haven't thought of anything better yet
        String firstEnvInfo = responseData.getEnvironmentInfo()[0];
        assertFalse(firstEnvInfo.contains("Built"));
        assertTrue(firstEnvInfo.contains("VersionCmd"));
    }

    @Test
    @Category(UnitTest.class)
    public void getServicesVersionNoBuildFile() throws Exception {
        // Use PowerMock here so we can mock a static method.
        PowerMockito.mockStatic(BuildInfo.class);

        // simulate the build.info file not existing
        PowerMockito.when(BuildInfo.getInfo()).thenThrow(IOException.class);

        VersionInfo responseData =
                target("/about/servicesVersionInfo").request(MediaType.APPLICATION_JSON).get(VersionInfo.class);

        assertEquals("unknown", responseData.getName());
        assertEquals("unknown", responseData.getVersion());
        assertEquals("unknown", responseData.getBuiltBy());
    }

    @Test
    @Category(UnitTest.class)
    public void testCoreVersionParser() throws Exception {
        AboutResource aboutResource = new AboutResource();
        Method privateMethod = AboutResource.class.getDeclaredMethod("parseCoreVersionOutOf", String.class, Boolean.TYPE);
        privateMethod.setAccessible(true);

        String returnValue = (String) privateMethod.invoke(aboutResource,
                "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", false);
        assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "This is just a line" + System.lineSeparator() +
                        "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", true);
        assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "14:27:08.974 WARN ...rc/main/cpp/hoot/core/Hoot.cpp( 135) " + System.lineSeparator() + " "
                        + "Cannot load library HootHadoop: (libhdfs.so.0: cannot open shared object file: No such file or directory)" + System.lineSeparator() +
                        "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant",
                true);
        assertEquals("Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant", returnValue);

        returnValue = (String) privateMethod.invoke(aboutResource,
                "14:27:08.974 WARN ...rc/main/cpp/hoot/core/Hoot.cpp( 135) " + System.lineSeparator() + " "
                        + "Cannot load library HootHadoop: (libhdfs.so.0: cannot open shared object file: No such file or directory)",
                true);

        assertEquals("Unable to determine!", returnValue);
    }
}
