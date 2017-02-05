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
package hoot.services.command;

import static hoot.services.HootProperties.*;
import static org.junit.Assert.*;

import java.io.File;
import java.lang.reflect.Method;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesSpringTestConfig;
import hoot.services.testsupport.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@ActiveProfiles("test")
@Transactional
public class RasterToTilesResourceTest {
    private static File homeFolder;
    private static String tileServerPath;
    private static String original_HOME_FOLDER;
    private static String original_TILE_SERVER_PATH;

    @BeforeClass
    public static void beforeClass() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homeFolder = new File(FileUtils.getTempDirectory(), "RasterToTilesResourceTest");
        FileUtils.forceMkdir(homeFolder);
        assertTrue(homeFolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());

        //tileServerPath=$(homeFolder)/ingest/processed

        original_TILE_SERVER_PATH = TILE_SERVER_PATH;
        File processedFolder = new File(homeFolder, "ingest/processed");
        FileUtils.forceMkdir(processedFolder);
        assertTrue(processedFolder.exists());
        tileServerPath = processedFolder.getAbsolutePath();
        assertNotNull(tileServerPath);
        assertTrue(!tileServerPath.isEmpty());
        HootCustomPropertiesSetter.setProperty("TILE_SERVER_PATH", processedFolder.getAbsolutePath());
    }

    @AfterClass
    public static void oneTimeTearDown() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
        HootCustomPropertiesSetter.setProperty("TILE_SERVER_PATH", original_TILE_SERVER_PATH);
    }

    @Test
    @Category(UnitTest.class)
    public void TestIngestOSMResource() throws Exception {
        String processScriptName = RASTER_TO_TILES;

        assertNotNull(processScriptName);
        assertTrue(!processScriptName.isEmpty());

        JSONObject oExpected = new JSONObject();
        oExpected.put("caller", "RasterToTilesService");
        oExpected.put("exec", processScriptName);

        JSONArray params = new JSONArray();
        JSONObject param = new JSONObject();
        param.put("RASTER_OUTPUT_DIR", tileServerPath);
        params.add(param);

        param = new JSONObject();
        param.put("INPUT", "test");
        params.add(param);

        param = new JSONObject();
        param.put("ZOOM_LIST", "0-1 2-3");
        params.add(param);

        param = new JSONObject();
        param.put("RASTER_SIZE", "500");
        params.add(param);

        long userId = MapUtils.insertUser();
        String mapId = Long.toString(MapUtils.insertMap(userId));

        param = new JSONObject();
        param.put("MAP_ID", mapId);
        params.add(param);

        oExpected.put("params", params);
        oExpected.put("exectype", "make");
        oExpected.put("erroraswarning", "true");

        //JSONObject command = new RasterToTilesCommandFactory().createExternalCommand(mapId, "test@test");

        //assertEquals(oExpected, command);
    }

    @Test
    @Category(UnitTest.class)
    public void TestGetZoomInfo() throws Exception {
        RasterToTilesCommandFactory rts = new RasterToTilesCommandFactory();
        Method getZoomInfoMethod = RasterToTilesCommandFactory.class.getDeclaredMethod("getZoomInfo", double.class);
        getZoomInfoMethod.setAccessible(true);

        JSONObject oActual = (JSONObject) getZoomInfoMethod.invoke(rts, 0.025);

        assertEquals("0-1 2-3 4-5 6-7 8-9 10-11 12-13 14-15 16-17", oActual.get("zoomlist").toString());
        assertEquals(500, oActual.get("rastersize"));
    }
}
