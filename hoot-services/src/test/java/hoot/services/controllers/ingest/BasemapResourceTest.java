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
package hoot.services.controllers.ingest;

import java.io.File;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class BasemapResourceTest {
    @SuppressWarnings("unused")
    @BeforeClass
    public static void oneTimeSetup() {
        new BasemapResource();
        org.junit.Assert.assertNotNull(BasemapResource._tileServerPath);
        org.junit.Assert.assertTrue(BasemapResource._tileServerPath.length() > 0);

        org.junit.Assert.assertNotNull(BasemapResource._ingestStagingPath);
        org.junit.Assert.assertTrue(BasemapResource._ingestStagingPath.length() > 0);
    }

    @Test
    @Category(UnitTest.class)
    public void TestgetBasemapList() throws Exception {
        BasemapResource mapRes = new BasemapResource();
        File f = new File(BasemapResource._tileServerPath + "/BASEMAP/TestMap");
        FileUtils.forceMkdir(f);

        JSONObject cont = new JSONObject();
        cont.put("jobid", "123-456-789");
        cont.put("path", "/projects/hoot/ingest/processed/BASEMAP/TestMap");

        File file = new File(BasemapResource._ingestStagingPath + "/BASEMAP/TestMap.enabled");
        FileUtils.writeStringToFile(file, cont.toJSONString());

        File f2 = new File(BasemapResource._tileServerPath + "/BASEMAP/TestMap2");
        FileUtils.forceMkdir(f2);

        File file2 = new File(BasemapResource._ingestStagingPath + "/BASEMAP/TestMap2.enabled");
        FileUtils.writeStringToFile(file2, cont.toJSONString());

        JSONArray res = mapRes._getBasemapList();
        boolean found = false;
        for (Object oMap : res) {
            JSONObject map = (JSONObject) oMap;
            if (map.get("name").toString().equals("TestMap")) {
                found = true;
                break;
            }
        }
        org.junit.Assert.assertTrue(found);
        FileUtils.forceDelete(f);
        FileUtils.forceDelete(f2);
    }

    @Test
    @Category(UnitTest.class)
    public void TestToggleBasemap() throws Exception {
        BasemapResource mapRes = new BasemapResource();
        File f = new File(BasemapResource._ingestStagingPath + "/BASEMAP/controltest.enabled");
        FileUtils.touch(f);
        mapRes._toggleBaseMap("controltest", false);
        f = new File(BasemapResource._ingestStagingPath + "/BASEMAP/controltest.disabled");
        org.junit.Assert.assertTrue(f.exists());

        mapRes._toggleBaseMap("controltest", true);

        f = new File(BasemapResource._ingestStagingPath + "/BASEMAP/controltest.enabled");
        org.junit.Assert.assertTrue(f.exists());

        FileUtils.forceDelete(f);
    }

    @Test
    @Category(UnitTest.class)
    public void TestDeleteBasemap() throws Exception {
        String testMapName = "testmap";

        BasemapResource mapRes = new BasemapResource();
        File dir = new File(BasemapResource._tileServerPath + "/BASEMAP/" + testMapName);
        FileUtils.forceMkdir(dir);

        org.junit.Assert.assertTrue(dir.exists());

        File controlFile = new File(BasemapResource._ingestStagingPath + "/BASEMAP/" + testMapName + ".enabled");
        FileUtils.touch(controlFile);
        org.junit.Assert.assertTrue(controlFile.exists());

        mapRes._deleteBaseMap(testMapName);

        org.junit.Assert.assertFalse(dir.exists());

        org.junit.Assert.assertFalse(controlFile.exists());
    }
}
