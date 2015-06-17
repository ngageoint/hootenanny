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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.io.IOException;
import java.util.UUID;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;

import hoot.services.geo.BoundingBox;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableItemsResponse;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.WebResource;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class ReviewResourceGetTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewResourceGetTest.class);
  private QReviewItems reviewItems = QReviewItems.reviewItems;

  public ReviewResourceGetTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }

  /*
   * This tests mimics the behavior defined in
     $HOOT_HOME/hoot-services/src/main/src/main/java/hoot/services/controllers/review/GetReviewableItemsProcesslet.java
     So, any changes to that definition must also be reflected in this method and vice versa.
   */
  private ReviewableItemsResponse execGet(final String mapId, final String numItems,
    final String highestReviewScoreFirst, final String reviewScoreThresholdMinimum,
    final String geospatialBounds) throws Exception
  {
    WebResource resource = resource().path("/review");
    if (mapId != null)
    {
      resource = resource.queryParam("mapId", mapId);
    }
    if (numItems != null)
    {
      resource = resource.queryParam("numItems", numItems);
    }
    if (highestReviewScoreFirst != null)
    {
      resource = resource.queryParam("highestReviewScoreFirst", highestReviewScoreFirst);
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
        .get(ReviewableItemsResponse.class);
  }

  @Test
  @Category(UnitTest.class)
  public void testGet() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, null, null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetWithNumItems() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), "2", null, null, null);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(2, response.getNumItemsRequested());
    Assert.assertEquals(2, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(2, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.054806,38.9010842,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetDontSortByScore() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ReviewableItemsResponse response =
      execGet(String.valueOf(mapId), null, "false", null, null);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(false, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.750459, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-65), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{5c131c00-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetWithScoreMin() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ReviewableItemsResponse response =
      execGet(String.valueOf(mapId), "10", null, "1.0", null);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(1.0, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(10, response.getNumItemsRequested());
    Assert.assertEquals(2, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(2, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.054806,38.9010842,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetByBounds() throws Exception
  {
    final BoundingBox queryBounds = ReviewTestUtils.createTestQueryBounds();
    ReviewTestUtils.createPreparedData(resource());

    final ReviewableItemsResponse response =
      execGet(String.valueOf(mapId), "20", null, null, queryBounds.toServicesString());

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(20, response.getNumItemsRequested());
    //discrepancy here between this and ReviewResourceGetStatsTest::testGetByBounds; this method
    //is correct and ReviewResourceGetStatsTest::testGetByBounds is wrong
    Assert.assertEquals(7, response.getNumItemsReturned());
    Assert.assertEquals(queryBounds, new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(7, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.relationIds.get((long)-3), reviewableItem.getId());
    Assert.assertEquals(ElementType.Relation.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.939793, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{0c9f69f0-dad5-11e3-9c1a-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    // removing since precison can change
    /*Assert.assertEquals(
      "-77.05624759999999,38.900034,-77.0515,38.9040842", itemsToReview[0].getDisplayBounds());*/
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-67), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{de285618-a4df-4536-b218-5009c25f7107}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-41), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05625099999999,38.9009821,-77.0515,38.9045", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-20), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[2];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-41), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05625099999999,38.9009821,-77.0515,38.9045", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-18), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[3];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-40), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.056242,38.9009821,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-20), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[4];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-40), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.056242,38.9009821,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-18), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[5];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-44), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{51cd8890-a312-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    // removing since precision can change
   /* Assert.assertEquals(
      "-77.05625069999999,38.900034,-77.0515,38.9045", reviewableItem.getDisplayBounds());*/
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.relationIds.get((long)-1), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Relation.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{7ea9ac60-a310-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[6];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.750459, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-65), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{5c131c00-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetDataNotPrepared() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      execGet(String.valueOf(mapId), null, null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.PRECONDITION_FAILED.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains(
          "The map with ID: " + mapId + " has not had review data prepared for it."));
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetNoReviewableData() throws Exception
  {
    //make a review map and job entry but don't write the reviewed data
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.COMPLETE.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, null, null, null);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(0, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    //this behavior is different than the wps
    Assert.assertNull(itemsToReview);
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
      execGet(String.valueOf(mapId), null, null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.PRECONDITION_FAILED.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains(
          "The prepare job with ID: " + jobId + " for the map with ID: " + mapId +
          " has a status of " + jobStatus.toString()));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetPrepareJobStillRunning() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.RUNNING);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetPrepareJobFailed() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.FAILED);
  }

  @Test(expected=UniformInterfaceException.class)
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
      execGet(String.valueOf(mapId + 1), null, null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("No record exists"));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMissingMapIdParam() throws Exception
  {
    try
    {
      execGet(null, null, null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("Invalid input parameter"));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyMapIdParam() throws Exception
  {
    try
    {
      execGet("", null, null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("Invalid input parameter"));
      throw e;
    }
  }

  //See ReviewResourceGetStatsTest::testGetEmptyReviewScoreMinParam for an explanation of why
  //this test doesn't throw an error.
  @Test
  @Category(UnitTest.class)
  public void testGetEmptyNumItemsParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, numItems = 1
    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), "", null, null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  //see ReviewResourceGetStatsTest::testGetInvalidReviewScoreMinParam for an explanation of why
  //this throws a 404 instead of a 400
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetInvalidNumItemsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "abc", null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetNumItemsParamOutOfRange() throws Exception
  {
    final int maxReviewGetSize =
      Integer.parseInt(
        HootProperties.getInstance().getProperty(
          "reviewGetMaxReviewSize", HootProperties.getDefault("reviewGetMaxReviewSize")));
    try
    {
      execGet(String.valueOf(mapId), String.valueOf(maxReviewGetSize + 1), null, null, null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("out of the allowable range"));
      throw e;
    }
  }

  //See ReviewResourceGetStatsTest::testGetEmptyReviewScoreMinParam for an explanation of why
  //this test doesn't throw an error.
  @Test
  @Category(UnitTest.class)
  public void testGetEmptyReviewScoreSortingParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, highestReviewScoreFirst = true (or testGet)
    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, "", null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  //Jersey will convert boolean input params that were passed in with invalid string values to false
  @Test//(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetInvalidReviewScoreSortingParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, highestReviewScoreFirst = false
    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, "abc", null, null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(false, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.750459, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-65), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{5c131c00-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  //See ReviewResourceGetStatsTest::testGetEmptyReviewScoreMinParam for an explanation of why
  //this test doesn't throw an error.
  @Test
  @Category(UnitTest.class)
  public void testGetEmptyReviewScoreMinParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, reviewScoreThresholdMinimum = 0.5
    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, null, "", null);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  //see ReviewResourceGetStatsTest::testGetInvalidReviewScoreMinParam for an explanation of why
  //this throws a 404 instead of a 400
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetInvalidReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, "abc", null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetReviewScoreMinParamOutOfRange() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, "1.01", null);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("out of the allowable range"));
      throw e;
    }
  }

  /*
   * This is a little strange b/c we're catching this from the method that prepares the exec call.
     This should reflect reality, though, b/c the exec call simulates how the processlet is actually
     called in real operation.
   *
   * @todo probably need a better way to test this
   */
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyBoundsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, null, "");
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid input parameter"));
      throw e;
    }
  }

  /*
   * @todo need a better way to test this; see testGetEmptyBounds
   */
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetInvalidBoundsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, null, "-181,-90,180,90");
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid minimum longitude value"));
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetWithReviewedItemsPresent() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //mark the first item as reviewed
    if (
    		new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId),reviewItems)
    		.where(reviewItems.reviewableItemId.eq("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}"))
    		.set(reviewItems.reviewStatus, DbUtils.review_status_enum.reviewed)
    		.execute() <= 0)
    {
      throw new Exception("Error updating item.");
    }

    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), null, null, null, null);

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(UnitTest.class)
  public void testGetReviewItemAssociatedWithElementThatNoLongerExists() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //Currently, we simply don't return a reviewable item if its OSM element is deleted before it
    //has been marked as reviewed.  It *might* make more sense to throw an error in this situation.
    //Or, eventually, we could set FKC between the review and OSM tables.
    //long temp = ReviewTestUtils.wayIds.get((long)-40);
    //now delete one of the element records associated with a reviewable item
    ReviewTestUtils.deleteReviewableItem();

    final ReviewableItemsResponse response = execGet(String.valueOf(mapId), "20", null, null, null);

    //we should get back two fewer items from this query
    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(14, itemsToReview.length);
  }
}
