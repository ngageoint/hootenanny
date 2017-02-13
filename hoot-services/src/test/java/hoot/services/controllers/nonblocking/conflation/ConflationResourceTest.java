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
package hoot.services.controllers.nonblocking.conflation;

import static org.junit.Assert.*;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;

import java.io.File;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.UnitTest;
import hoot.services.controllers.nonblocking.NonblockingJobResource;
import hoot.services.geo.BoundingBox;
import hoot.services.job.Job;
import hoot.services.models.osm.Map;
import hoot.services.testsupport.HootCustomPropertiesSetter;


public class ConflationResourceTest {

    // TODO: This test needs to be reworked
    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testProcess() {
        String params = "{\"INPUT1_TYPE\":\"DB\",\"INPUT1\":\"DcGisRoads\",\"INPUT2_TYPE\":\"DB\",\"INPUT2\":\"DcTigerRoads\",";
        params += "\"OUTPUT_NAME\":\"Merged_Roads_e0d\",\"CONFLATION_TYPE\":\"Reference\",\"MATCH_THRESHOLD\":\"0.6\",\"MISS_THRESHOLD\":\"0.6\",\"USER_EMAIL\":\"test@test.com\",\"COLLECT_STATS\":\"false\"}";

        String jobArgs = "\"exec\":\"makeconflate\",\"params\":[{\"USER_EMAIL\":\"test@test.com\"},{\"CONFLATION_TYPE\":\"Reference\"},"
                + "{\"MATCH_THRESHOLD\":\"0.6\"},{\"INPUT1_TYPE\":\"DB\"},{\"MISS_THRESHOLD\":\"0.6\"},{\"INPUT2_TYPE\":\"DB\"},"
                + "{\"INPUT2\":\"DcTigerRoads\"},{\"INPUT1\":\"DcGisRoads\"},{\"COLLECT_STATS\":\"false\"},{\"OUTPUT_NAME\":\"Merged_Roads_e0d\"},"
                + "{\"IS_BIG\":\"false\"}],\"exectype\":\"make\"}]";

        ConflationResource conflationResource = new ConflationResource();
        ConflationResource spy = Mockito.spy(conflationResource);
        doNothing().when(spy).processJob(any());

        //JobId jobId = spy.process(params);
        //assertNotNull(jobId);
        //assertNotNull(jobId.getJobid());
        verify(spy).process(Matchers.eq(params));

        ArgumentCaptor<Job> argCaptor = ArgumentCaptor.forClass(Job.class);

        //verify(spy).processChainJob(argCaptor.capture());

        //assertEquals(jobId, argCaptor.getValue().getJobId());
        //ChainJob chainJob = new ChainJob(jobId.getJobid(), null);

        //verify(spy).processChainJob(Matchers.chainJob);
        //verify(spy).processChainJob(Matchers.matches(resp.getJobid()), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessOsmApiDbInput() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/conflation/ConflationResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), Charset.defaultCharset());

            ConflationResource spy = Mockito.spy(new ConflationResource());

            doNothing().when((NonblockingJobResource) spy).processJob(any());
            Mockito.doReturn(true).when(spy).mapExists(anyLong());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            //String jobId = spy.process(inputParams).getJobid();

            // just checking that the request made it the command runner w/o
            // error and that the map tag got added; testProcess checks the generated input at a more
            // detailed level

            //verify(spy).processChainJob(Matchers.matches(jobId),
                    // wasn't able to get the mockito matcher to take the timestamp
            // regex...validated the
            // regex externally, and it looks correct
                    /*
                     * Matchers.matches("\"osm_api_db_export_time\":\"" +
                     * DbUtils.TIME_STAMP_REGEX + "\"")
                     */
            //AdditionalMatchers.and(Matchers.contains("osm_api_db_export_time"),
            //        Matchers.contains("\"conflateaoi\":\"0.0,0.0,0.0,0.0\"")));
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    // An OSM API DB input must always be a reference layer. Default ref layer = 1.

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testOsmApiDbInputAsSecondary() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils.readFileToString(new File(Thread
                    .currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/conflation/ConflationResourceTestOsmApiDbInputAsSecondaryInput.json")
                    .getPath()), Charset.defaultCharset());

            ConflationResource spy = Mockito.spy(new ConflationResource());
            doNothing().when((NonblockingJobResource) spy).processJob(any());
            spy.process(inputParams);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("OSM_API_DB not allowed as secondary input type"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testOsmApiDbInputAsSecondary2() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils.readFileToString(new File(Thread
                    .currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/conflation/ConflationResourceTestOsmApiDbInputAsSecondary2Input.json")
                    .getPath()), Charset.defaultCharset());

            ConflationResource spy = Mockito.spy(new ConflationResource());
            doNothing().when((NonblockingJobResource) spy).processJob(any());
            spy.process(inputParams);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("OSM_API_DB not allowed as secondary input type"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testConflateOsmApiDbMissingMap() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
            String inputParams = FileUtils.readFileToString(new File(Thread
                .currentThread()
                .getContextClassLoader()
                .getResource("hoot/services/controllers/nonblocking/conflation/ConflationResourceTestProcessOsmApiDbMissingMapInput.json")
                .getPath()), Charset.defaultCharset());

            ConflationResource spy = Mockito.spy(new ConflationResource());

            doNothing().when((NonblockingJobResource) spy).processJob(any());
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.process(inputParams);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.INTERNAL_SERVER_ERROR.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains("Error during process call!  Params: "));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testConflateOsmApiDbNotEnabled() throws Exception {
        try {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.FALSE);
            String inputParams = FileUtils.readFileToString(new File(Thread.currentThread()
                    .getContextClassLoader()
                    .getResource("hoot/services/controllers/nonblocking/conflation/ConflationResourceTestProcessOsmApiDbInputInput.json")
                    .getPath()), Charset.defaultCharset());

            ConflationResource spy = Mockito.spy(new ConflationResource());

            doNothing().when((NonblockingJobResource) spy).processJob(any());
            List<Long> mapIds = new ArrayList<>();
            mapIds.add(1L);
            BoundingBox mapBounds = new BoundingBox(0.0, 0.0, 0.0, 0.0);
            Mockito.doReturn(mapBounds).when(spy).getMapBounds(any(Map.class));

            spy.process(inputParams);
        }
        catch (WebApplicationException e) {
            assertEquals(Status.INTERNAL_SERVER_ERROR.getStatusCode(), e.getResponse().getStatus());
            assertTrue(e.getResponse().getEntity().toString().contains(
                    "Attempted to conflate an OSM API database data source but OSM API database support is disabled"));
            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("OSM_API_DB_ENABLED", Boolean.TRUE);
        }
    }
}