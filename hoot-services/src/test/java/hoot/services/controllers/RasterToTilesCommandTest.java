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
package hoot.services.controllers;

import static hoot.services.HootProperties.TILE_SERVER_PATH;
import static org.junit.Assert.assertEquals;

import java.lang.reflect.Method;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PowerMockIgnore;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import hoot.services.UnitTest;
import hoot.services.utils.DbUtils;


@RunWith(PowerMockRunner.class)
@PrepareForTest({DbUtils.class, RasterToTilesCommand.class})
@PowerMockIgnore("javax.management.*")
public class RasterToTilesCommandTest {
    private static final String MAP_NAME = "test";
    private static final Double MAX_DELTA = 0.025;

/*
    @Test
    @Category(UnitTest.class)
    public void testRasterToTilesCommandCreation() throws Exception {
        PowerMockito.mockStatic(DbUtils.class);

        JSONObject extents = new JSONObject();
        extents.put("minlon", 20.1d);
        extents.put("maxlon", 20.2d);
        extents.put("minlat", 20.3d);
        extents.put("maxlat", 20.4d);
        PowerMockito.when(RasterToTilesCommand.class, "getExtents", 1L).thenReturn(extents);

        JSONObject zoomInfo = new JSONObject();
        zoomInfo.put("zoomlist", "list");
        zoomInfo.put("rastersize", 200);
        PowerMockito.when(RasterToTilesCommand.class, "getZoomInfo", 0.09999999999999787d).thenReturn(zoomInfo);

        BDDMockito.given(DbUtils.getRecordIdForInputString(MAP_NAME, QMaps.maps, QMaps.maps.id, QMaps.maps.displayName)).willReturn(1L);

        RasterToTilesCommand rasterToTilesCommand = new RasterToTilesCommand(MAP_NAME, "test@test");

        String actualRasterToTilesCommand = rasterToTilesCommand.toJSONString();
        String expectedRasterToTilesCommand = "";

        assertEquals(expectedRasterToTilesCommand, actualRasterToTilesCommand);
    }
*/
    @Test
    @Category(UnitTest.class)
    public void testCreateCommadParams() throws Exception {
        Method createCommandParamsMethod =
                RasterToTilesCommand.class.getDeclaredMethod("createCommandParams", String.class,
                        String.class, int.class, String.class, long.class);
        createCommandParamsMethod.setAccessible(true);

        JSONArray actualCommandArgs = (JSONArray) createCommandParamsMethod.invoke(null,
                "test", "0-1 2-3", 500, "test@test", 1L);

        String expectedCommandArgs =
                 "[{\"RASTER_OUTPUT_DIR\":\"" + TILE_SERVER_PATH.replace("/", "\\/") + "\"}," +
                  "{\"INPUT\":\"test\"}," +
                  "{\"ZOOM_LIST\":\"0-1 2-3\"}," +
                  "{\"RASTER_SIZE\":\"500\"}," +
                  "{\"MAP_ID\":\"1\"}," +
                  "{\"USER_EMAIL\":\"test@test\"}," +
                  "{\"DB_URL\":\"hootapidb:\\/\\/${dbUserId}:${dbPassword}@${dbHost}:${dbPort}\\/${dbName}\"}]";

        assertEquals(expectedCommandArgs, actualCommandArgs.toJSONString());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetZoomInfo() throws Exception {
        Method getZoomInfoMethod = RasterToTilesCommand.class.getDeclaredMethod("getZoomInfo", double.class);
        getZoomInfoMethod.setAccessible(true);

        JSONObject actualZoomInfo = (JSONObject) getZoomInfoMethod.invoke(null, MAX_DELTA);
        String expectedZoomList = "0-1 2-3 4-5 6-7 8-9 10-11 12-13 14-15 16-17";
        String actualZoomList = actualZoomInfo.get("zoomlist").toString();

        assertEquals(expectedZoomList, actualZoomList);

        Integer expectedRastersize = 500;
        Integer actualRastersize = (Integer) actualZoomInfo.get("rastersize");

        assertEquals(expectedRastersize, actualRastersize);
    }
}
