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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job.custom.HGIS;

import java.util.List;

import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import hoot.services.UnitTest;
import hoot.services.models.review.custom.HGIS.PrepareForValidationRequest;

//import hoot.services.models.review.custom.HGIS.PrepareForValidationResponse;


public class HGISReviewResourceTest {

    @Test
    @Category(UnitTest.class)
    public void TestPrepareItemsForValidationReview() throws Exception {

        HGISReviewResource real = new HGISReviewResource();
        HGISReviewResource spy = Mockito.spy(real);

        org.mockito.Mockito.doReturn(true).when(spy)._mapExists(org.mockito.Matchers.anyString());

        ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

        Mockito.doNothing().when(spy).postChainJobRquest(org.mockito.Matchers.anyString(), argCaptor.capture());
        PrepareForValidationRequest request = new PrepareForValidationRequest();
        request.setSourceMap("testSrc1");
        request.setOutputMap("out1");
        /* PrepareForValidationResponse resp = */ spy.prepareItemsForValidationReview(request);

        List<String> args = argCaptor.getAllValues();
        String param = args.get(0);
        JSONParser parser = new JSONParser();
        JSONArray result = (JSONArray) parser.parse(param);

        JSONObject command = (JSONObject) result.get(0);

        Assert.assertEquals(command.get("exec"), "custom/HGIS/PrepareForValidation.sh");
        Assert.assertEquals(command.get("exectype"), "bash");
        Assert.assertNotNull(command.get("params"));
        JSONArray arr = (JSONArray) command.get("params");
        String connStr = spy._generateDbMapParam("testSrc1");
        Assert.assertEquals(((JSONObject) arr.get(0)).get("SOURCE"), connStr);
        connStr = spy._generateDbMapParam("out1");
        Assert.assertEquals(((JSONObject) arr.get(1)).get("OUTPUT"), connStr);

        command = (JSONObject) result.get(1);

        Assert.assertEquals(command.get("class"), "hoot.services.controllers.job.custom.HGIS.HGISReviewResource");
        Assert.assertEquals(command.get("method"), "updateMapsTag");
        Assert.assertEquals(command.get("exectype"), "reflection");
        Assert.assertNotNull(command.get("params"));
        arr = (JSONArray) command.get("params");

        Assert.assertEquals(((JSONObject) arr.get(0)).get("value"), "out1");

    }

    @Test(expected = javax.ws.rs.WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidSource() throws Exception {

        try {
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap(null);
            request.setOutputMap("out1");
            HGISReviewResource real = new HGISReviewResource();
            real.prepareItemsForValidationReview(request);
        }
        catch (javax.ws.rs.WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }

    }

    @Test(expected = javax.ws.rs.WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidOutput() throws Exception {

        try {
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap("source");
            request.setOutputMap(null);
            HGISReviewResource real = new HGISReviewResource();
            real.prepareItemsForValidationReview(request);
        }
        catch (javax.ws.rs.WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }
    }

    @Test(expected = javax.ws.rs.WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidNoMap() throws Exception {

        try {
            HGISReviewResource real = new HGISReviewResource();
            HGISReviewResource spy = Mockito.spy(real);

            org.mockito.Mockito.doReturn(false).when(spy)._mapExists(org.mockito.Matchers.anyString());

            ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

            Mockito.doNothing().when(spy).postJobRquest(org.mockito.Matchers.anyString(), argCaptor.capture());
            PrepareForValidationRequest request = new PrepareForValidationRequest();
            request.setSourceMap("testSrc1");
            request.setOutputMap("out1");
            /* PrepareForValidationResponse resp = */ spy.prepareItemsForValidationReview(request);
        }
        catch (javax.ws.rs.WebApplicationException e) {
            Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }

    }
}
