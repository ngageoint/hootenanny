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
package hoot.services.controllers.job;

import static hoot.services.HootProperties.*;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.anyLong;

import java.io.File;
import java.util.HashMap;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Mockito;

import hoot.services.UnitTest;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Map;
import hoot.services.testsupport.HootCustomPropertiesSetter;


public class DeriveChangesetResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testExportToOsmApiDb() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot.services.controllers.job/ExportResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());
            Mockito.doNothing().when((JobControllerBase) spy).postJobRequest(anyString(), anyString());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            String mapBoundsStr = mapBounds.toServicesString();
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            String commandArgs = spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams)).toString();

            assertTrue(commandArgs.contains("{\"input\":\"MyTestMap\"}"));
            assertTrue(commandArgs.contains("{\"outputtype\":\"osm_api_db\"}"));
            assertTrue(commandArgs.contains("{\"removereview\":\"false\"}"));
            assertTrue(commandArgs.contains("{\"inputtype\":\"db\"}"));
            assertTrue(commandArgs.contains("{\"temppath\":"));
            assertTrue(commandArgs.contains("{\"changesetsourcedatatimestamp\":\"" + exportTime + "\"}"));
            assertTrue(commandArgs.contains("{\"changesetaoi\":\"" + mapBoundsStr + "\"}"));
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testExportToOsmApiDbWithTranslation() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "true");
            String inputParams = FileUtils
                    .readFileToString(new File(
                            Thread.currentThread()
                                    .getContextClassLoader()
                                    .getResource("hoot.services.controllers.job/ExportResourceTestExportToOsmApiDbWithTranslationInput.json")
                                    .getPath()));

            ExportJobResource spy = Mockito.spy(new ExportJobResource());

            Mockito.doNothing().when((JobControllerBase) spy).postJobRequest(anyString(), anyString());
            Long mapId = 1L;
            Mockito.doReturn(mapId).when(spy).getMapIdByName(anyString());
            java.util.Map<String, String> mapTags = new HashMap<>();
            String exportTime = "2016-05-04 10:15";
            mapTags.put("osm_api_db_export_time", exportTime);
            Mockito.doReturn(mapTags).when(spy).getMapTags(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.getExportToOsmApiDbCommandArgs(ExportJobResource.parseParams(inputParams), (JSONObject) new JSONParser().parse(inputParams)).toString();
        }
        catch (WebApplicationException e) {
            assertEquals(Response.Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString()
                    .contains("Custom translation not allowed when exporting to OSM API database."));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", "false");
        }
    }
}