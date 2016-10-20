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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.info;

import static hoot.services.HootProperties.ERROR_LOG_PATH;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.io.File;
import java.io.InputStream;
import java.io.StringWriter;
import java.net.URL;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesJerseyTestAbstract;


public class ErrorLogTest extends HootServicesJerseyTestAbstract {
    private static final File testFolder = new File(FileUtils.getTempDirectory(), "ErrorLogTest");
    private static String original_ERROR_LOG_PATH;
    private static String original_TEMP_OUTPUT_PATH;

    @BeforeClass
    public static void beforeClass() throws Exception {
        URL inputUrl = ErrorLogTest.class.getResource("/hoot/services/info/catalina.out");
        File dest = new File(testFolder, "catalina.out");
        FileUtils.copyURLToFile(inputUrl, dest);

        original_ERROR_LOG_PATH = ERROR_LOG_PATH;
        original_TEMP_OUTPUT_PATH = TEMP_OUTPUT_PATH;
        HootCustomPropertiesSetter.setProperty("ERROR_LOG_PATH", dest.getAbsolutePath());
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", FileUtils.getTempDirectory().getAbsolutePath());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteQuietly(testFolder);
        HootCustomPropertiesSetter.setProperty("ERROR_LOG_PATH", original_ERROR_LOG_PATH);
        HootCustomPropertiesSetter.setProperty("TEMP_OUTPUT_PATH", original_TEMP_OUTPUT_PATH);
    }

    @Test
    @Category(UnitTest.class)
    public void testGenerateExportLog() throws Exception {
        Response responseData =
                target("/logging/export").request(MediaType.APPLICATION_OCTET_STREAM).get(Response.class);

        InputStream is = responseData.readEntity(InputStream.class);
        StringWriter writer = new StringWriter();
        IOUtils.copy(is, writer);
        String theString = writer.toString();
        assertFalse(theString.isEmpty());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetErrorLog() throws Exception {
        Response responseData =
                target("/logging/debuglog").request(MediaType.APPLICATION_JSON).get(Response.class);

        String theString = responseData.readEntity(String.class);
        assertFalse(theString.isEmpty());
        assertEquals(282637, theString.length());
    }
}
