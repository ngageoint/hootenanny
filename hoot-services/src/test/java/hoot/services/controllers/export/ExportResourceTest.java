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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.HootProperties.TRANSLATION_EXT_PATH;
import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertSame;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.command.common.ZIPDirectoryContentsCommand;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.Job;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ExportResourceTest extends OSMResourceTestAbstract {

    private static String originalTEMP_OUTPUT_PATH;

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
        exportParams.setInput(String.valueOf(mapId));
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setInputType("file");
        exportParams.setIncludeHootTags(false);
        exportParams.setAppend(false);

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
    public void testExportOSMPBFResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("osm.pbf");
        exportParams.setInput(String.valueOf(mapId));
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setInputType("file");
        exportParams.setIncludeHootTags(false);
        exportParams.setAppend(false);

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
    public void testExportTilesResource() throws Exception {
        long userId = MapUtils.insertUser();
        MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams jobParams = new ExportParams();
        jobParams.setOutputName("output");
        jobParams.setAppend(false);
        jobParams.setTextStatus(false);
        jobParams.setInputType("file");
        jobParams.setIncludeHootTags(false);
        jobParams.setOutputType("tiles");
        jobParams.setInput(String.valueOf(mapId));
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
        assertEquals(2, job.getCommands().length);
        assertSame(CalculateTilesCommand.class, job.getCommands()[0].getClass());
        assertSame(ZIPDirectoryContentsCommand.class, job.getCommands()[1].getClass());
    }

    @Test
    @Category(UnitTest.class)
    public void testExportShapeResource() throws Exception {
        ExportParams exportParams = new ExportParams();
        exportParams.setOutputType("shp");
        exportParams.setTranslation("translations/TDSv40.js");
        exportParams.setInput(String.valueOf(mapId));
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setIncludeHootTags(false);

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
        exportParams.setInput(String.valueOf(mapId));
        exportParams.setOutputName("output");
        exportParams.setAppend(false);
        exportParams.setTextStatus(false);
        exportParams.setInputType("file");
        exportParams.setIncludeHootTags(false);

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

        String transExtPath = HOME_FOLDER + "/" + "/translations-local/script/utp";
        if (!TRANSLATION_EXT_PATH.isEmpty()) {
            transExtPath = TRANSLATION_EXT_PATH;
        }

        List<java.util.Map<String, Object>> expectedObject = new ArrayList<java.util.Map<String, Object>>();
        java.util.Map<String, Object> obj = new HashMap<String, Object>();
        obj.put("name", "TDS");
        obj.put("description", "LTDS 4.0");
        expectedObject.add(obj);

        obj = new HashMap<String, Object>();
        obj.put("name", "MGCP");
        obj.put("description", "MGCP");
        expectedObject.add(obj);

        File file = new File(transExtPath);
        if (file.exists() && file.isDirectory()) {
            obj = new HashMap<String, Object>();
            obj.put("name", "UTP");
            obj.put("description", "UTP");
            expectedObject.add(obj);
        }
        String expected = expectedObject.toString();

        assertEquals(expected, result);
    }
}
