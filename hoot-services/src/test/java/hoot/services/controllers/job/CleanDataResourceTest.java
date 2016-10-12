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

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.verify;

import java.util.List;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.UnitTest;


public class CleanDataResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testProcess() throws Exception {
        ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

        CleanDataResource spy = Mockito.spy(new CleanDataResource());
        Mockito.doNothing().when((JobControllerBase) spy).postJobRequest(anyString(), argCaptor.capture());

        String params = "{\"INPUT_TYPE\":\"DB\",\"INPUT\":\"DcGisRoads\",\"OUTPUT_TYPE\":\"DB\",\"OUTPUT\":\"DcGisRoadsOUt5\"}";
        JobId resp = spy.process(params);

        JSONParser parser = new JSONParser();

        List<String> values = argCaptor.getAllValues();
        JSONObject jsonObject = (JSONObject) parser.parse(values.get(0));

//        String expected = "[{\"INPUT\":\"DcGisRoads\"},{\"OUTPUT\":\"DcGisRoadsOUt5\"},{\"INPUT_TYPE\":\"DB\"},{\"OUTPUT_TYPE\":\"DB\"}]";
//        assertEquals(parser.parse(expected), jsonObject.get("params"));

        String expected = "makecleandata";
        assertEquals(expected, jsonObject.get("exec").toString());

        expected = "make";
        assertEquals(expected, jsonObject.get("exectype").toString());

        verify(spy).postJobRequest(Matchers.matches(resp.getJobid()), anyString());
    }
}
