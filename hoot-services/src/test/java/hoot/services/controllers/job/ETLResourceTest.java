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

import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.verify;

import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.UnitTest;


public class ETLResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testProcess() throws Exception {
        String params = "{\"TRANSLATION\":\"translations/MGCP.js\",\"INPUT_TYPE\":\"OSM\",\"INPUT\":\"test-files/ToyTestA.osm\",\"INPUT_NAME\":\"ToyTestA\"}";

        String jobArgs = ",\"exec\":\"makeetl\",\"params\":[{\"INPUT\":\"test-files\\/ToyTestA.osm\"},{\"INPUT_TYPE\":\"OSM\"},";
        jobArgs += "{\"TRANSLATION\":\"translations\\/MGCP.js\"},{\"INPUT_NAME\":\"ToyTestA\"}],\"exectype\":\"make\"}";

        ETLResource spy = Mockito.spy(new ETLResource());
        Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
        Response resp = spy.process(params);
        String result = resp.getEntity().toString();
        JSONParser parser = new JSONParser();
        JSONObject o = (JSONObject) parser.parse(result);
        String jobId = o.get("jobid").toString();
        verify(spy).postJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }
}
