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

import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertSame;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.Job;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class HGISReviewResourceTest extends HootServicesJerseyTestAbstract {

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

    @Test
    public void testPrepareForValidation() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);

        PrepareForValidationRequest prepareForValidationRequest = new PrepareForValidationRequest();
        prepareForValidationRequest.setSourceMap(String.valueOf(mapId));
        prepareForValidationRequest.setOutputMap("output_of_" + String.valueOf(mapId));

        Response response = target("/review/custom/HGIS/preparevalidation")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(prepareForValidationRequest), Response.class);

        assertNotNull(response);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertSame(HGISPrepareForValidationCommand.class, job.getCommands()[0].getClass());
        assertSame(UpdateMapTagsCommand.class, job.getCommands()[1].getClass());
    }
}
