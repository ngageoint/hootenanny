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

import static hoot.services.HootProperties.HOME_FOLDER;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.net.URL;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.hamcrest.CoreMatchers;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesJerseyTestAbstract;


public class AdvancedOptResourceTest extends HootServicesJerseyTestAbstract {

    private static File homeFolder;
    private static String original_HOME_FOLDER;

    @BeforeClass
    public static void beforeClass() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homeFolder = new File(FileUtils.getTempDirectory(), "AdvancedOptResourceTest");
        FileUtils.forceMkdir(homeFolder);
        assertTrue(homeFolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());

        File testFolder = new File(homeFolder, "conf");
        FileUtils.forceMkdir(testFolder);

        File dest = new File(testFolder, "conflationRefOps.json");
        URL inputUrl = AdvancedOptResourceTest.class.getResource("/hoot/services/controllers/info/conflationRefOps.json");
        FileUtils.copyURLToFile(inputUrl, dest);

        dest = new File(testFolder, "conflateAdvOps.json");
        inputUrl = AdvancedOptResourceTest.class.getResource("/hoot/services/controllers/info/conflateAdvOps.json");
        FileUtils.copyURLToFile(inputUrl, dest);

        dest = new File(testFolder, "conflationAverageOps.json");
        inputUrl = AdvancedOptResourceTest.class.getResource("/hoot/services/controllers/info/conflationAverageOps.json");
        FileUtils.copyURLToFile(inputUrl, dest);

        dest = new File(testFolder, "conflationHorizontalOps.json");
        inputUrl = AdvancedOptResourceTest.class.getResource("/hoot/services/controllers/info/conflationHorizontalOps.json");
        FileUtils.copyURLToFile(inputUrl, dest);

        dest = new File(testFolder, "ConfigOptions.asciidoc");
        inputUrl = AdvancedOptResourceTest.class.getResource("/hoot/services/controllers/info/ConfigOptions.asciidoc");
        FileUtils.copyURLToFile(inputUrl, dest);
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeReference() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "reference")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("reference_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeAverage() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "average")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("average_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeHorizontal() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "horizontal")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("horizontal_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeAdvanced() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "custom")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("hoot_cleaning_options"));
    }
}
