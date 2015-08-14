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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewItems;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.MarkItemsReviewedResponse;
import hoot.services.models.review.ReviewedItem;
import hoot.services.models.review.ReviewedItems;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.XmlDocumentBuilder;

import org.apache.commons.lang3.StringUtils;
import org.junit.Assert;
//import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import com.mysema.query.sql.SQLQuery;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.WebResource;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class ReviewResourceMarkTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log =
    LoggerFactory.getLogger(ReviewResourceMarkTest.class);

  private QReviewItems reviewItems = QReviewItems.reviewItems;

  public ReviewResourceMarkTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }

  private MarkItemsReviewedResponse execMark(final String mapId, final String markAll,
    final MarkItemsReviewedRequest markItemsReviewedRequest) throws Exception
  {
    WebResource resource = resource().path("/review");
    if (mapId != null)
    {
      resource = resource.queryParam("mapId", mapId);
    }
    if (StringUtils.trimToNull(markAll) != null)
    {
      resource = resource.queryParam("markAll", markAll);
    }
    return
      resource
        .type(MediaType.APPLICATION_JSON)
        .accept(MediaType.APPLICATION_JSON)
        .put(MarkItemsReviewedResponse.class, markItemsReviewedRequest);
  }

  private MarkItemsReviewedResponse testMark(final boolean prepareData,
    final boolean markAll, final boolean verifyData) throws Exception
  {
    if (prepareData)
    {
      ReviewTestUtils.createPreparedData(resource());
    }

    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(markAll),
        ReviewTestUtils.createMarkItemsReviewedRequest());

    if (verifyData)
    {
      //Changeset uploading is heavily tested in the OSM controller tests, so not testing that here.

      Assert.assertNotNull(responseData);
      Assert.assertTrue(responseData.getChangesetId() > -1);
      ReviewTestUtils.changesetId = responseData.getChangesetId();
      if (!markAll)
      {
        Assert.assertEquals(5, responseData.getNumItemsMarkedReviewed());
        //verify the changeset data was written
        ReviewTestUtils.verifyDataMarkedAsReviewed(true);
      }
      else
      {
        Assert.assertEquals(16, responseData.getNumItemsMarkedReviewed());
        //verify all data was written
        ReviewTestUtils.verifyAllDataMarkedAsReviewed();
      }
    }

    return responseData;
  }

  @Test
  @Category(UnitTest.class)
  public void testMark() throws Exception
  {
    testMark(true, false, true);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkAll() throws Exception
  {
    testMark(true, true, true);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkMissingReviewedItemsParam() throws IOException, Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(null);
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(ReviewTestUtils.createReviewedItemsChangeset()));

    try
    {
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("Invalid input parameter"));
      ReviewTestUtils.verifyDataNotMarkedAsReviewed();
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkMissingReviewedItemsChangesetParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(null);

    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    ReviewTestUtils.verifyDataMarkedAsReviewed(false);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkEmptyReviewedItemsChangesetParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset("");

    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    ReviewTestUtils.verifyDataMarkedAsReviewed(false);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkMapDoesntExist() throws  IOException, Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    try
    {
      execMark(String.valueOf(mapId + 2), String.valueOf(false),
        ReviewTestUtils.createMarkItemsReviewedRequest());
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("No record exists"));

      ReviewTestUtils.verifyDataNotMarkedAsReviewed();

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkEmptyMapIdParam() throws  IOException, Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    try
    {
      execMark("", String.valueOf(false), ReviewTestUtils.createMarkItemsReviewedRequest());
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("Invalid input parameter"));

      ReviewTestUtils.verifyDataNotMarkedAsReviewed();

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkMissingMapIdParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    try
    {
      execMark(null, String.valueOf(false), ReviewTestUtils.createMarkItemsReviewedRequest());
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains("Invalid input parameter"));

      ReviewTestUtils.verifyDataNotMarkedAsReviewed();

      throw e;
    }
  }

  //see ReviewResourceGetStatsTest::testGetInvalidReviewScoreMinParam for why this doesn't throw
  @Test
  @Category(UnitTest.class)
  public void testMarkEmptyMarkAllParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, markAll = false
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), "", ReviewTestUtils.createMarkItemsReviewedRequest());

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    ReviewTestUtils.verifyDataMarkedAsReviewed(true);
  }

  //see ReviewResourceGetStatsTest::testGetInvalidReviewScoreMinParam for why this doesn't throw
  @Test
  @Category(UnitTest.class)
  public void testMarkMissingMarkAllParam() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, markAll = false
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), "", ReviewTestUtils.createMarkItemsReviewedRequest());

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    ReviewTestUtils.verifyDataMarkedAsReviewed(true);
  }

  //see ReviewResourceGetStatsTest::testGetInvalidReviewScoreMinParam for why this doesn't throw
  @Test
  @Category(UnitTest.class)
  public void testMarkInvalidMarkAllParam() throws  IOException, Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //effectively, markAll = false
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), "", ReviewTestUtils.createMarkItemsReviewedRequest());

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    ReviewTestUtils.verifyDataMarkedAsReviewed(true);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkDataNotPrepared() throws Exception
  {
    MarkItemsReviewedRequest request = new MarkItemsReviewedRequest();
    //This has to be set to a dummy object, b/c ReviewTestUtils.createReviewedItems depends upon
    //the data already being prepared (TODO: change that).  We can get around it by setting markAll
    //to true, and then the map not being prepared will fail before the empty reviewed items object
    //is parsed.
    request.setReviewedItems(new ReviewedItems());
    request.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(ReviewTestUtils.createReviewedItemsChangeset()));

    try
    {
      execMark(String.valueOf(mapId), String.valueOf(true), request);
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

  private void testMarkAgainstInvalidJobStatus(final JOB_STATUS jobStatus) throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    final String jobId = ReviewTestUtils.jobId;

    DbUtils.updateJobStatus(
      jobId, jobStatus.toInt(), jobStatus.equals(JOB_STATUS.COMPLETE), "", conn);
    Assert.assertEquals(DbUtils.getJobStatus(jobId, conn), jobStatus.toInt());

    try
    {
      execMark(
        String.valueOf(mapId), String.valueOf(false),
        ReviewTestUtils.createMarkItemsReviewedRequest());
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.PRECONDITION_FAILED.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains(
          "The prepare job with ID: " + jobId + " for the map with ID: " + mapId +
          " has a status of " + jobStatus.toString()));

      //update the status to keep the verify method happy
      DbUtils.updateJobStatus(jobId, JOB_STATUS.COMPLETE.toInt(), true, "", conn);
      ReviewTestUtils.verifyDataNotMarkedAsReviewed();

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkPrepareJobFailed() throws Exception
  {
    testMarkAgainstInvalidJobStatus(JOB_STATUS.FAILED);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkPrepareJobStillRunning() throws Exception
  {
    testMarkAgainstInvalidJobStatus(JOB_STATUS.RUNNING);
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testMarkPrepareJobUnknown() throws Exception
  {
    testMarkAgainstInvalidJobStatus(JOB_STATUS.UNKNOWN);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkUpdateItemReviewedMissingUuidTag() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final Document changesetUploadRequest =
      XmlDocumentBuilder.parse(
       "<osmChange>" +
         "<create/>" +
         "<modify>" +
           "<node visible=\"true\" id=\"" + ReviewTestUtils.nodeIds.get((long)-64) +
             "\" version=\"1\" changeset=\"\" " +
             "lat=\"38.9125841913319113\" lon=\"-77.0629999999999831\">" +
             "<tag k=\"hoot:review:score\" v=\"0.750459\"/>" +
             "<tag k=\"hoot:review:uuid\" v=\"{5c131c00-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             //missing uuid tag
             "<tag k=\"hoot:review:needs\" v=\"yes\"/>" +
           "</node>" +
         "</modify>" +
         "<delete/>" +
       "</osmChange>");
    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(changesetUploadRequest));

    //this should not fail, and should log a warning about the reviewable item whose uuid tag
    //was missing
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    //not going to verify all the tags and items, since its being done in many other methods
    Assert.assertEquals(
      5,
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
    		.where(
    				reviewItems.mapId.eq(mapId)
    				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed))
    				)
    		.count()

    		);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkUpdateItemReviewedEmptyUuidTag() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final Document changesetUploadRequest =
      XmlDocumentBuilder.parse(
       "<osmChange>" +
         "<create/>" +
         "<modify>" +
           "<node visible=\"true\" id=\"" + ReviewTestUtils.nodeIds.get((long)-64) +
             "\" version=\"1\" changeset=\"\" " +
             "lat=\"38.9125841913319113\" lon=\"-77.0629999999999831\">" +
             "<tag k=\"hoot:review:score\" v=\"0.750459\"/>" +
             "<tag k=\"hoot:review:uuid\" v=\"{5c131c00-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             //empty uuid tag
             "<tag k=\"uuid\" v=\"\"/>" +
             "<tag k=\"hoot:review:needs\" v=\"yes\"/>" +
           "</node>" +
         "</modify>" +
         "<delete/>" +
       "</osmChange>");
    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(changesetUploadRequest));

    //this should not fail, and should log a warning about the reviewable item whose uuid tag
    //was removed
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    //not going to verify all the tags and items, since its being done in many other methods
    Assert.assertEquals(
      5,
  		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
  		.where(
  				reviewItems.mapId.eq(mapId)
  				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed))
  				)
  		.count()

    		);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkUpdateItemWithNoReviewTags() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final Document changesetUploadRequest =
      XmlDocumentBuilder.parse(
       "<osmChange>" +
         "<create/>" +
         "<modify>" +
           "<node visible=\"true\" id=\"" + ReviewTestUtils.nodeIds.get((long)-64) +
             "\" version=\"1\" changeset=\"\" " +
             "lat=\"38.9125841913319113\" lon=\"-77.0629999999999831\">" +
             //"<tag k=\"hoot:review:score\" v=\"0.750459\"/>" +
             //"<tag k=\"hoot:review:uuid\" v=\"{5c131c00-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             "<tag k=\"uuid\" v=\"{25e1aa70-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             //Even though this tag indicates no review necessary, the convention is to only mark
             //an item reviewed if it has no review tags at all (tags beginning with "hoot:review").
             //"<tag k=\"hoot:review:needs\" v=\"yes\"/>" +
           "</node>" +
         "</modify>" +
         "<delete/>" +
       "</osmChange>");
    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(changesetUploadRequest));

    //this should not fail, and should log a warning about the reviewable item whose uuid tag
    //was removed
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    //not going to verify all the tags and items, since its being done in many other methods

    Assert.assertEquals(
      5,

  		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
  		.where(
  				reviewItems.mapId.eq(mapId)
  				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed))
  				)
  		.count()
    		);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkUpdateItemWithEmptyReviewTags() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final Document changesetUploadRequest =
      XmlDocumentBuilder.parse(
       "<osmChange>" +
         "<create/>" +
         "<modify>" +
           "<node visible=\"true\" id=\"" + ReviewTestUtils.nodeIds.get((long)-64) +
             "\" version=\"1\" changeset=\"\" " +
             "lat=\"38.9125841913319113\" lon=\"-77.0629999999999831\">" +
             "<tag k=\"hoot:review:score\" v=\"0.750459\"/>" +
             "<tag k=\"hoot:review:uuid\" v=\"{5c131c00-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             "<tag k=\"uuid\" v=\"{25e1aa70-a30f-11e3-a5e2-0800200c9a66}\"/>" +
             //empty review tag
             "<tag k=\"hoot:review:needs\" v=\"\"/>" +
           "</node>" +
         "</modify>" +
         "<delete/>" +
       "</osmChange>");
    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(changesetUploadRequest));

    //this should not fail, and should log a warning about the reviewable item whose uuid tag
    //was removed
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);

    Assert.assertNotNull(responseData);
    //not going to verify all the tags and items, since its being done in many other methods
    Assert.assertEquals(
      5,
  		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
  		.where(
  				reviewItems.mapId.eq(mapId)
  				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed))
  				)
  		.count()
    		);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkItemReviewedItemNotReviewable() throws Exception
  {
    //If an item is marked as reviewed that is not a reviewable item in the services database, we
    //don't fail.  A warning is logged only.
    ReviewTestUtils.createPreparedData(resource());

    //add an additional reviewed item that is invalid b/c its not actually reviewable
    ReviewedItems reviewedItems = ReviewTestUtils.createReviewedItems();
    ReviewedItem[] reviewedItemsArr = reviewedItems.getReviewedItems();
    List<ReviewedItem> reviewedItemsList = new ArrayList<ReviewedItem>(Arrays.asList(reviewedItemsArr));
    ReviewedItem invalidReviewedItem = new ReviewedItem();
    invalidReviewedItem.setId(ReviewTestUtils.nodeIds.get((long)-65));
    invalidReviewedItem.setType(ElementType.Node.toString().toLowerCase());
    invalidReviewedItem.setReviewedAgainstId(ReviewTestUtils.nodeIds.get((long)-64));
    invalidReviewedItem.setReviewedAgainstType(ElementType.Node.toString().toLowerCase());
    reviewedItemsList.add(invalidReviewedItem);
    reviewedItemsArr = reviewedItemsList.toArray(new ReviewedItem[]{});
    reviewedItems.setReviewedItems(reviewedItemsArr);

    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(reviewedItems);
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(ReviewTestUtils.createReviewedItemsChangeset()));

    //should just skip updating the item, log a warning, and not fail
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false),
        ReviewTestUtils.createMarkItemsReviewedRequest());

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    Assert.assertEquals(5, responseData.getNumItemsMarkedReviewed());
    ReviewTestUtils.verifyDataMarkedAsReviewed(true);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkItemReviewedThatWasPreviouslyDeleted() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    //delete one of the reviewable items
    ReviewTestUtils.deleteReviewableItem();
    MarkItemsReviewedRequest request = ReviewTestUtils.createMarkItemsReviewedRequest();
    //this changeset needs to be set null here b/c it conflicts with the previous delete operation
    request.setReviewedItemsChangeset(null);

    //should just skip updating the item, log a warning, and not fail
    final MarkItemsReviewedResponse responseData =
      execMark(String.valueOf(mapId), String.valueOf(false), request);

    Assert.assertNotNull(responseData);
    ReviewTestUtils.changesetId = responseData.getChangesetId();
    Assert.assertEquals(5, responseData.getNumItemsMarkedReviewed());
    ReviewTestUtils.verifyDataMarkedAsReviewed(false);
  }

  @Test
  @Category(UnitTest.class)
  public void testMarkItemReviewedAgainstInvalidReviewAgainstItem() throws Exception
  {
    //If an item is marked as reviewed against something it wasn't supposed to be reviewed against
    //in the services database, we don't fail.  A warning is logged only.
    ReviewTestUtils.createPreparedData(resource());

    //add an additional reviewed item that is invalid b/c its review against item is incorrect
    ReviewedItems reviewedItems = ReviewTestUtils.createReviewedItems();
    ReviewedItem[] reviewedItemsArr = reviewedItems.getReviewedItems();
    List<ReviewedItem> reviewedItemsList =
      new ArrayList<ReviewedItem>(Arrays.asList(reviewedItemsArr));
    ReviewedItem invalidReviewedItem = reviewedItemsList.get(0);
    //set item to be reviewed against itself
    invalidReviewedItem.setReviewedAgainstId(ReviewTestUtils.nodeIds.get((long)-64));
    invalidReviewedItem.setReviewedAgainstType(ElementType.Node.toString().toLowerCase());
    reviewedItemsList.set(0, invalidReviewedItem);
    reviewedItemsArr = reviewedItemsList.toArray(new ReviewedItem[]{});
    reviewedItems.setReviewedItems(reviewedItemsArr);

    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(ReviewTestUtils.createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(ReviewTestUtils.createReviewedItemsChangeset()));

    //the request should fail and nothing should be written
    try
    {
      execMark(String.valueOf(mapId), String.valueOf(false), markItemsReviewedRequest);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(e.getResponse().getEntity(String.class).contains(
        "Error parsing unique ID tag"));

      ReviewTestUtils.verifyDataNotMarkedAsReviewed();

      throw e;
    }
  }
}
