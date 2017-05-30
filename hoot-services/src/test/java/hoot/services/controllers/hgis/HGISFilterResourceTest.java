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
public class HGISFilterResourceTest extends HootServicesJerseyTestAbstract {

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidSource() throws Exception {
        FilterNonHgisPoisRequest request = new FilterNonHgisPoisRequest();
        request.setSource(null);
        request.setOutput("out1");
        HGISFilterResource real = new HGISFilterResource();
        real.filterNonHgisPois(request);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidOutput() throws Exception {
        FilterNonHgisPoisRequest request = new FilterNonHgisPoisRequest();
        request.setSource("source");
        request.setOutput(null);
        HGISFilterResource real = new HGISFilterResource();
        real.filterNonHgisPois(request);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidSourceMap() throws Exception {
        HGISFilterResource real = new HGISFilterResource();
        HGISFilterResource spy = Mockito.spy(real);

        FilterNonHgisPoisRequest request = new FilterNonHgisPoisRequest();
        request.setSource("");
        request.setOutput("out1");

        spy.filterNonHgisPois(request);
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void TestInvalidOutputMap() throws Exception {
        HGISFilterResource real = new HGISFilterResource();
        HGISFilterResource spy = Mockito.spy(real);

        FilterNonHgisPoisRequest request = new FilterNonHgisPoisRequest();
        request.setSource("source");
        request.setOutput("");

        spy.filterNonHgisPois(request);
    }

    @Test
    public void testFilterNonHgisPois() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);

        FilterNonHgisPoisRequest filterNonHgisPoisRequest = new FilterNonHgisPoisRequest();
        filterNonHgisPoisRequest.setSource(String.valueOf(mapId));
        filterNonHgisPoisRequest.setOutput("output_of_" + String.valueOf(mapId));

        Response response = target("/filter/custom/HGIS/filternonhgispois")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(filterNonHgisPoisRequest), Response.class);

        assertNotNull(response);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(1, job.getCommands().length);
        assertSame(FilterNonHGISPOIsCommand.class, job.getCommands()[0].getClass());
    }
}
