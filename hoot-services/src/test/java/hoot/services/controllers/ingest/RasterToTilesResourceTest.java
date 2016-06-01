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

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


public class RasterToTilesResourceTest {

    @Test
    @Category(UnitTest.class)
    public void TestIngestOSMResource() throws Exception {
        String processScriptName = HootProperties.getProperty("RasterToTiles");
        org.junit.Assert.assertNotNull(processScriptName);
        org.junit.Assert.assertTrue(processScriptName.length() > 0);

        RasterToTilesService rts = new RasterToTilesService();

        org.junit.Assert.assertNotNull(RasterToTilesService._tileServerPath);
        org.junit.Assert.assertTrue(RasterToTilesService._tileServerPath.length() > 0);

        JSONObject oExpected = new JSONObject();
        oExpected.put("caller", "RasterToTilesService");
        oExpected.put("exec", processScriptName);

        JSONArray params = new JSONArray();
        JSONObject param = new JSONObject();
        param.put("RASTER_OUTPUT_DIR", RasterToTilesService._tileServerPath);
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

        param = new JSONObject();
        param.put("MAP_ID", "1");
        params.add(param);

        oExpected.put("params", params);

        oExpected.put("exectype", "make");
        oExpected.put("erroraswarning", "true");

        String actual = rts._createCommand("test", "0-1 2-3", 500, 1);

        org.junit.Assert.assertEquals(oExpected.toString(), actual);
    }

    @Test
    @Category(UnitTest.class)
    public void TestGetZoomInfo() throws Exception {
        RasterToTilesService rts = new RasterToTilesService();
        JSONObject oActual = rts._getZoomInfo(0.025);

        org.junit.Assert.assertEquals("0-1 2-3 4-5 6-7 8-9 10-11 12-13 14-15 16-17",
                oActual.get("zoomlist").toString());
        org.junit.Assert.assertEquals(500, oActual.get("rastersize"));
    }
}
