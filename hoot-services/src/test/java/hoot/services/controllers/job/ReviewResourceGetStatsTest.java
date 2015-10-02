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
package hoot.services.controllers.job;

import java.io.IOException;
import java.util.UUID;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.dml.SQLInsertClause;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.WebResource;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class ReviewResourceGetStatsTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewResourceGetStatsTest.class);

  public ReviewResourceGetStatsTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }

  private ReviewableItemsStatistics execGet(final String mapId,
    final String reviewScoreThresholdMinimum, final String geospatialBounds) throws Exception
  {
    WebResource resource = resource().path("/review/statistics");
    if (mapId != null)
    {
      resource = resource.queryParam("mapId", mapId);
    }
    if (reviewScoreThresholdMinimum != null)
    {
      resource = resource.queryParam("reviewScoreThresholdMinimum", reviewScoreThresholdMinimum);
    }
    if (geospatialBounds != null)
    {
      resource = resource.queryParam("geospatialBounds", geospatialBounds);
    }
    return
      resource
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewableItemsStatistics.class);
  }

  @Test
  @Category(UnitTest.class)
  public void testGet() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ReviewableItemsStatistics response = execGet(String.valueOf(mapId), null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(13, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetWithScoreMin() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ReviewableItemsStatistics response = execGet(String.valueOf(mapId), "0.928", null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(5, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetByBounds() throws Exception
  {
    final BoundingBox queryBounds = ReviewTestUtils.createTestQueryBounds();
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ReviewableItemsStatistics response =
      execGet(String.valueOf(mapId),null, queryBounds.toServicesString());

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    //TODO: fix - this should be returning 7 instead; I believe it's ignoring way -44, whose review
    //against item is a relation with a node inside the query bounds; oddly enough the review get
    //resource test performs this query correctly, and they use nearly the same query code
    Assert.assertEquals(6, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetNoReviewableData() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    //make a review map and job entry but don't write the reviewed data
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.COMPLETE.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    final ReviewableItemsStatistics response = execGet(String.valueOf(mapId), null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(0, response.getNumReviewableItems());
    Assert.assertEquals(0, response.getNumReviewedItems());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetDataNotPrepared() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
    	ReviewableItemsStatistics stat = execGet(String.valueOf(mapId),null, null);
    	Assert.assertTrue(stat.getMapId() == mapId);
      Assert.assertTrue(stat.getNumReviewableItems() == 0);
      Assert.assertTrue(stat.getNumReviewedItems() == 0);
      Assert.assertTrue(stat.getNumTotalItems() == 0);
    }
    catch (UniformInterfaceException e)
    {
      throw e;
    }
  }

  private void testGetAgainstInvalidJobStatus(final JOB_STATUS jobStatus) throws Exception
  {
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, jobStatus.toInt(), conn);

    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();
    try
    {
      ReviewableItemsStatistics stat = execGet(String.valueOf(mapId), null, null);
      Assert.assertTrue(stat.getMapId() == mapId);
      Assert.assertTrue(stat.getNumReviewableItems() == 0);
      Assert.assertTrue(stat.getNumReviewedItems() == 0);
      Assert.assertTrue(stat.getNumTotalItems() == 0);
    }
    catch (UniformInterfaceException e)
    {
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetPrepareJobStillRunning() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.RUNNING);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetPrepareJobFailed() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.FAILED);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetPrepareJobUnknown() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.UNKNOWN);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId + 1), null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("No record exists"));

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMissingMapIdParam() throws Exception
  {
    try
    {
      execGet(null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("No record exists with ID"));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyMapIdParam() throws Exception
  {
    try
    {
      execGet("", null, null);
    }
    catch (UniformInterfaceException e)
    {
    	 Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
       Assert.assertTrue(
         e.getResponse().getEntity(String.class).contains("No record exists with ID"));
      throw e;
    }
  }
  
  //This behavior differs between Jersey and WPS.  Jersey will  automatically insert the default
  //value for empty numeric and boolean params.  This would be a 400 with WPS.
  @Test
  @Category(UnitTest.class)
  public void testGetEmptyReviewScoreMinParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, reviewScoreMinThreshold = 0.0
    final ReviewableItemsStatistics response = execGet(String.valueOf(mapId), "", null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(16, response.getNumReviewableItems());
    Assert.assertEquals(0, response.getNumReviewedItems());
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetInvalidReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "abc", null);
    }
    catch (UniformInterfaceException e)
    {
      //the input parameters for a Jersey request are more strongly typed than for a WPS request
      //so this unparsable type throws a 404, rather than a 400, b/c Jersey doesn't recognize the
      //request destination with unparsable input params.
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetReviewScoreMinParamOutOfRange() throws Exception
  {
    try
    {
    	ReviewableItemsStatistics stat = execGet(String.valueOf(mapId), "1.01", null);
    	Assert.assertTrue(stat.getMapId() == mapId);
      Assert.assertTrue(stat.getNumReviewableItems() == 0);
      Assert.assertTrue(stat.getNumReviewedItems() == 0);
      Assert.assertTrue(stat.getNumTotalItems() == 0);
    }
    catch (UniformInterfaceException e)
    {
      throw e;
    }
  }

  /*
   * This is a little strange b/c we're catching this from the method that prepares the exec call.
     This should reflect reality, though, b/c the exec call simulates how the processlet is actually
     called in real operation.
   */
  @Test
  @Category(UnitTest.class)
  public void testGetEmptyBoundsParam() throws Exception
  {
    try
    {
    	ReviewableItemsStatistics stat = execGet(String.valueOf(mapId), null, "");
    	Assert.assertTrue(stat.getMapId() == mapId);
      Assert.assertTrue(stat.getNumReviewableItems() == 0);
      Assert.assertTrue(stat.getNumReviewedItems() == 0);
      Assert.assertTrue(stat.getNumTotalItems() == 0);
    }
    catch (UniformInterfaceException e)
    {
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetInvalidBoundsParam() throws Exception
  {
    try
    {
      execGet(
        String.valueOf(mapId), null, "-181,-90,180,90");
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid coordinate"));
      throw e;
    }
  }
}
