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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.assertEquals;

import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.Job;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ConflationResourceTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testConflate() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);

        ConflateParams conflateParams = new ConflateParams();
        conflateParams.setInputType1("OSM");
        conflateParams.setInput1("1");
        conflateParams.setInputType2("DB");
        conflateParams.setInput2(String.valueOf(mapId));
        conflateParams.setOutputName("OutputLayer");
        conflateParams.setConflationType("Reference");
        conflateParams.setCollectStats(false);
        conflateParams.setBounds("20,30,40,50");
        conflateParams.setReferenceLayer("1");
        conflateParams.setAdvancedOptions("-D \"convert.bounding.box=0,0,0,0\"");

        Response response = target("conflation/execute")
                .queryParam("DEBUG_LEVEL", "info")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(conflateParams), Response.class);

        assertNotNull(response);

        Job job = super.getSubmittedJob();

        assertNotNull(job);

        assertEquals(3, job.getCommands().length);
        assertEquals(ConflateCommand.class, job.getCommands()[0].getClass());
        assertEquals(UpdateMapTagsCommand.class, job.getCommands()[1].getClass());

    }
}
