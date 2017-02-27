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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.hgis;

import static org.junit.Assert.assertEquals;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response.Status;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Mockito;

import hoot.services.UnitTest;


public class HGISReviewResourceTest {

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
    public void TestInvalidSourceMap() throws Exception {
        HGISReviewResource real = new HGISReviewResource();
        HGISReviewResource spy = Mockito.spy(real);

        PrepareForValidationRequest request = new PrepareForValidationRequest();
        request.setSourceMap("");
        request.setOutputMap("output");

        spy.prepareItemsForValidationReview(request);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidOutputMap() throws Exception {
        HGISReviewResource real = new HGISReviewResource();
        HGISReviewResource spy = Mockito.spy(real);

        PrepareForValidationRequest request = new PrepareForValidationRequest();
        request.setSourceMap("source");
        request.setOutputMap("");

        spy.prepareItemsForValidationReview(request);
    }
}
