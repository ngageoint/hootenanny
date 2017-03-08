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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.*;
import static org.junit.Assert.*;

import java.io.File;
import java.lang.reflect.Method;
import java.nio.charset.Charset;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.utils.HootCustomPropertiesSetter;


public class BasemapResourceTest {
    private static File homeFolder;
    private static String tileServerPath;
    private static String ingestStagingPath;
    private static String original_HOME_FOLDER;
    private static String original_TILE_SERVER_PATH;
    private static String original_INGEST_STAGING_PATH;

    @BeforeClass
    public static void beforeClass() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homeFolder = new File(FileUtils.getTempDirectory(), "BasemapResourceTest");
        FileUtils.forceMkdir(homeFolder);
        assertTrue(homeFolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());

        original_TILE_SERVER_PATH = BASEMAPS_TILES_FOLDER;
        File processedFolder = new File(homeFolder, "ingest/processed/BASEMAP");
        FileUtils.forceMkdir(processedFolder);
        assertTrue(processedFolder.exists());
        tileServerPath = processedFolder.getAbsolutePath();
        assertNotNull(tileServerPath);
        assertTrue(!tileServerPath.isEmpty());
        HootCustomPropertiesSetter.setProperty("BASEMAPS_TILES_FOLDER", processedFolder.getAbsolutePath());

        original_INGEST_STAGING_PATH = BASEMAPS_FOLDER;
        File ingestStagingFolder = new File(homeFolder, "/ingest/upload/BASEMAP");
        FileUtils.forceMkdir(ingestStagingFolder);
        assertTrue(ingestStagingFolder.exists());
        ingestStagingPath = ingestStagingFolder.getAbsolutePath();
        assertNotNull(ingestStagingPath);
        assertTrue(!ingestStagingPath.isEmpty());
        HootCustomPropertiesSetter.setProperty("BASEMAPS_FOLDER", ingestStagingFolder.getAbsolutePath());
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
        HootCustomPropertiesSetter.setProperty("BASEMAPS_TILES_FOLDER", original_TILE_SERVER_PATH);
        HootCustomPropertiesSetter.setProperty("BASEMAPS_FOLDER", original_INGEST_STAGING_PATH);
    }

    @Test
    @Category(UnitTest.class)
    public void TestgetBasemapList() throws Exception {
        File f = new File(tileServerPath + "/TestMap");
        FileUtils.forceMkdir(f);

        JSONObject cont = new JSONObject();
        cont.put("jobid", "123-456-789");
        cont.put("path", "/projects/hoot/ingest/processed/BASEMAP/TestMap");

        File file = new File(ingestStagingPath + "/TestMap.enabled");
        FileUtils.writeStringToFile(file, cont.toJSONString(), Charset.defaultCharset());

        File f2 = new File(tileServerPath + "/TestMap2");
        FileUtils.forceMkdir(f2);

        File file2 = new File(ingestStagingPath + "/TestMap2.enabled");
        FileUtils.writeStringToFile(file2, cont.toJSONString(), Charset.defaultCharset());

        Method getBasemapListHelperMethod = BasemapResource.class.getDeclaredMethod("getBasemapListHelper");
        getBasemapListHelperMethod.setAccessible(true);
        JSONArray res = (JSONArray) getBasemapListHelperMethod.invoke(null);

        boolean found = false;
        for (Object oMap : res) {
            JSONObject map = (JSONObject) oMap;
            if (map.get("name").toString().equals("TestMap")) {
                found = true;
                break;
            }
        }

        assertTrue(found);
        FileUtils.forceDelete(f);
        FileUtils.forceDelete(f2);
    }

    @Test
    @Category(UnitTest.class)
    public void TestToggleBasemap() throws Exception {
        File file = new File(ingestStagingPath + "/controltest.enabled");
        FileUtils.touch(file);

        Method toggleBaseMapMethod = BasemapResource.class.getDeclaredMethod("toggleBaseMap", String.class, boolean.class);
        toggleBaseMapMethod.setAccessible(true);

        toggleBaseMapMethod.invoke(null, "controltest", false);
        file = new File(ingestStagingPath + "/controltest.disabled");
        assertTrue(file.exists());

        toggleBaseMapMethod.invoke(null, "controltest", true);
        file = new File(ingestStagingPath + "/controltest.enabled");
        assertTrue(file.exists());

        FileUtils.forceDelete(file);
    }

    @Test
    @Category(UnitTest.class)
    public void TestDeleteBasemap() throws Exception {
        String testMapName = "testmap";

        File dir = new File(tileServerPath + "/" + testMapName);
        FileUtils.forceMkdir(dir);

        assertTrue(dir.exists());

        File controlFile = new File(ingestStagingPath + "/" + testMapName + ".enabled");
        FileUtils.touch(controlFile);
        assertTrue(controlFile.exists());

        Method deleteBaseMapMethod = BasemapResource.class.getDeclaredMethod("deleteBaseMapHelper", String.class);
        deleteBaseMapMethod.setAccessible(true);
        deleteBaseMapMethod.invoke(null, testMapName);

        assertFalse(dir.exists());
        assertFalse(controlFile.exists());
    }
}
