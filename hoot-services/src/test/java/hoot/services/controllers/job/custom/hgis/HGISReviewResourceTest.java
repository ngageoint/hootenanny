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
package hoot.services.controllers.job.custom.hgis;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.UnitTest;
import hoot.services.controllers.job.JobControllerBase;


public class HGISReviewResourceTest {

    @Test
    @Category(UnitTest.class)
    public void TestPrepareItemsForValidationReview() throws Exception {
        HGISReviewResource real = new HGISReviewResource();
        HGISReviewResource spy = Mockito.spy(real);

        Mockito.doReturn(true).when(spy).mapExists(Matchers.anyString());

        ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

        Mockito.doNothing().when((JobControllerBase) spy).postChainJobRequest(Matchers.anyString(), argCaptor.capture());
        PrepareForValidationRequest request = new PrepareForValidationRequest();
        request.setSourceMap("testSrc1");
        request.setOutputMap("out1");

        spy.prepareItemsForValidationReview(request);

        List<String> args = argCaptor.getAllValues();
        String param = args.get(0);
        JSONParser parser = new JSONParser();
        JSONArray result = (JSONArray) parser.parse(param);

        JSONObject command = (JSONObject) result.get(0);

        assertEquals("custom/HGIS/PrepareForValidation.sh", command.get("exec"));
        assertEquals("bash", command.get("exectype"));
        assertNotNull(command.get("params"));

        JSONArray arr = (JSONArray) command.get("params");
        String connStr = spy.generateDbMapParam("testSrc1");

        assertEquals(((JSONObject) arr.get(0)).get("SOURCE"), connStr);
        connStr = spy.generateDbMapParam("out1");
        assertEquals(((JSONObject) arr.get(1)).get("OUTPUT"), connStr);

        command = (JSONObject) result.get(1);

        assertEquals("hoot.services.controllers.job.custom.hgis.HGISReviewResource", command.get("class"));
        assertEquals("updateMapsTag", command.get("method"));
        assertEquals("reflection", command.get("exectype"));
        assertNotNull(command.get("params"));

        arr = (JSONArray) command.get("params");

        assertEquals("out1", ((JSONObject) arr.get(0)).get("value"));
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidSource() throws Exception {
        try {
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap(null);
            request.setOutputMap("out1");
            HGISReviewResource real = new HGISReviewResource();
            real.prepareItemsForValidationReview(request);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidOutput() throws Exception {
        try {
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap("source");
            request.setOutputMap(null);
            HGISReviewResource real = new HGISReviewResource();
            real.prepareItemsForValidationReview(request);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidNoMap() throws Exception {
        try {
            HGISReviewResource real = new HGISReviewResource();
            HGISReviewResource spy = Mockito.spy(real);

            Mockito.doReturn(false).when(spy).mapExists(Matchers.anyString());

            ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

            Mockito.doNothing().when(spy).postJobRequest(Matchers.anyString(), argCaptor.capture());
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap("testSrc1");
            request.setOutputMap("out1");

            spy.prepareItemsForValidationReview(request);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }
    }
}
