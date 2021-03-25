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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.clipping;

import static junit.framework.TestCase.assertNotNull;
import static org.junit.Assert.assertEquals;

import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Test;

import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.job.Job;


public class ClipDatasetResourceTest extends HootServicesJerseyTestAbstract {

    @Test
    public void testClipDataset() throws Exception {
        ClipDatasetParams params = new ClipDatasetParams();
        params.setBounds("8,-105,39,-104");
        params.setInputName("test_input");
        params.setOutputName("test_output");

        Response response = target("clipdataset/execute")
                .queryParam("DEBUG_LEVEL", "info")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(params), Response.class);

        assertNotNull(response);

        Job job = super.getSubmittedJob();

        assertNotNull(job);
        assertEquals(2, job.getCommands().length);
        assertEquals(ClipDatasetCommand.class, job.getCommands()[0].getClass());
    }
}