package hoot.services.writers.review;

import java.io.File;
import java.io.IOException;
import java.util.List;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.ReviewItems;
import hoot.services.models.osm.Changeset;
import hoot.services.models.review.SetAllItemsReviewedRequest;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;
import hoot.services.validators.osm.ChangesetUploadXmlValidator;
import hoot.services.writers.osm.ChangesetDbWriter;

import org.apache.commons.io.FileUtils;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import com.mysema.query.sql.SQLQuery;
import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewItemsSynchronizerTest extends OsmResourceTestAbstract
{
	private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
	
	public ReviewItemsSynchronizerTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
	
	@Test
	@Category(UnitTest.class)
	public void testUpdateReviewItems() throws Exception
	{
	  ChangesetDbWriter changesetWriter = new ChangesetDbWriter(conn);
		
		//parse the initial create changeset from the test input file
		long changesetId = Changeset.insertNew(mapId, userId, conn);
		ReviewTestUtils.changesetId = changesetId;
    Document changesetDoc = 
    	(new ChangesetUploadXmlValidator()).parseAndValidate
    	  (FileUtils.readFileToString(
          new File(
            Thread.currentThread().getContextClassLoader().getResource(
              "hoot/services/review/AllDataTypesConflateOut.osm")
            .getPath()))
          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
    changesetWriter.write(mapId, changesetId, changesetDoc);
    
    ReviewTestUtils.populateReviewDataForAllDataTypes();
		
		//parse the first review changeset
	  changesetId = Changeset.insertNew(mapId, userId, conn);
	  ReviewTestUtils.changesetId = changesetId;
		changesetDoc = 
    	(new ChangesetUploadXmlValidator()).parseAndValidate
    	  (FileUtils.readFileToString(
          new File(
            Thread.currentThread().getContextClassLoader().getResource(
              "hoot/services/review/AllDataTypesReviewChangeset1.osm")
            .getPath()))
          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
		changesetWriter.write(mapId, changesetId, changesetDoc);
		
    ReviewItemsSynchronizer reviewSynchronizer =  
    	new ReviewItemsSynchronizer(conn, String.valueOf(mapId));
	  
		//synchronize the review items with the changeset contents
		reviewSynchronizer.updateReviewItems(
			changesetDoc, changesetWriter.getParsedElementIdsToElementsByType());
	  
	  //check the review db table contents
	  Assert.assertEquals(
	  	8, 
	  	new SQLQuery(conn, DbUtils.getConfiguration(mapId))
        .from(elementIdMappings)
        .where(elementIdMappings.mapId.eq(mapId))
	      .count());
	  List<ReviewItems> reviewRecords =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)
		    .where(reviewItems.mapId.eq(mapId))
  		  .orderBy(reviewItems.reviewId.asc())
  		  .list(reviewItems);
	  Assert.assertEquals(5, reviewRecords.size());
	  ReviewItems reviewRecord = reviewRecords.get(0);
	  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
	  Assert.assertEquals(
	  	"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", reviewRecord.getReviewableItemId());
	  Assert.assertEquals(
	  	"{6117767e-8a0b-5624-a599-fa50f96213a6}", reviewRecord.getReviewAgainstItemId());
	  reviewRecord = reviewRecords.get(1);
	  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
	  Assert.assertEquals(
	  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
	  Assert.assertEquals(
	  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
	  reviewRecord = reviewRecords.get(2);
	  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
	  Assert.assertEquals(
	  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
	  Assert.assertEquals(
	  	"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", reviewRecord.getReviewAgainstItemId());
	  reviewRecord = reviewRecords.get(3);
	  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
	  Assert.assertEquals(
	  	"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", reviewRecord.getReviewableItemId());
	  Assert.assertEquals(
	  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
	  reviewRecord = reviewRecords.get(4);
	  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
	  Assert.assertEquals(
	  	"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", reviewRecord.getReviewableItemId());
	  Assert.assertEquals(
	  	"{ca3ae25a-164e-27ca-5aef-77a4597c7c32}", reviewRecord.getReviewAgainstItemId());
	  
	  //check the database contents after the next four review changesets

	  changesetId = Changeset.insertNew(mapId, userId, conn);
	  ReviewTestUtils.changesetId = changesetId;
		changesetDoc = 
    	(new ChangesetUploadXmlValidator()).parseAndValidate
    	  (FileUtils.readFileToString(
          new File(
            Thread.currentThread().getContextClassLoader().getResource(
              "hoot/services/review/AllDataTypesReviewChangeset2.osm")
            .getPath()))
          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
		changesetWriter.write(mapId, changesetId, changesetDoc);
		reviewSynchronizer.updateReviewItems(
			changesetDoc, changesetWriter.getParsedElementIdsToElementsByType());
		
		Assert.assertEquals(
		  	8, 
		  	new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	        .from(elementIdMappings)
	        .where(elementIdMappings.mapId.eq(mapId))
		      .count());
		 reviewRecords =
	      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
			    .from(reviewItems)
			    .where(reviewItems.mapId.eq(mapId))
	  		  .orderBy(reviewItems.reviewId.asc())
	  		  .list(reviewItems);
		  Assert.assertEquals(5, reviewRecords.size());
		  reviewRecord = reviewRecords.get(0);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{6117767e-8a0b-5624-a599-fa50f96213a6}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(1);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(2);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(3);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(4);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{ca3ae25a-164e-27ca-5aef-77a4597c7c32}", reviewRecord.getReviewAgainstItemId());
		  
		  changesetId = Changeset.insertNew(mapId, userId, conn);
		  ReviewTestUtils.changesetId = changesetId;
			changesetDoc = 
	    	(new ChangesetUploadXmlValidator()).parseAndValidate
	    	  (FileUtils.readFileToString(
	          new File(
	            Thread.currentThread().getContextClassLoader().getResource(
	              "hoot/services/review/AllDataTypesReviewChangeset3.osm")
	            .getPath()))
	          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
			changesetWriter.write(mapId, changesetId, changesetDoc);
			reviewSynchronizer.updateReviewItems(
				changesetDoc, changesetWriter.getParsedElementIdsToElementsByType());
			
			Assert.assertEquals(
		  	8, 
		  	new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	        .from(elementIdMappings)
	        .where(elementIdMappings.mapId.eq(mapId))
		      .count());
		  reviewRecords =
	      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
			    .from(reviewItems)
			    .where(reviewItems.mapId.eq(mapId))
	  		  .orderBy(reviewItems.reviewId.asc())
	  		  .list(reviewItems);
		  Assert.assertEquals(5, reviewRecords.size());
		  reviewRecord = reviewRecords.get(0);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{6117767e-8a0b-5624-a599-fa50f96213a6}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(1);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(2);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(3);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(4);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{ca3ae25a-164e-27ca-5aef-77a4597c7c32}", reviewRecord.getReviewAgainstItemId());
		  
		  changesetId = Changeset.insertNew(mapId, userId, conn);
		  ReviewTestUtils.changesetId = changesetId;
			changesetDoc = 
	    	(new ChangesetUploadXmlValidator()).parseAndValidate
	    	  (FileUtils.readFileToString(
	          new File(
	            Thread.currentThread().getContextClassLoader().getResource(
	              "hoot/services/review/AllDataTypesReviewChangeset4.osm")
	            .getPath()))
	          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
			changesetWriter.write(mapId, changesetId, changesetDoc);
			reviewSynchronizer.updateReviewItems(
				changesetDoc, changesetWriter.getParsedElementIdsToElementsByType());
			
			Assert.assertEquals(
		  	8, 
		  	new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	        .from(elementIdMappings)
	        .where(elementIdMappings.mapId.eq(mapId))
		      .count());
		  reviewRecords =
	      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
			    .from(reviewItems)
			    .where(reviewItems.mapId.eq(mapId))
	  		  .orderBy(reviewItems.reviewId.asc())
	  		  .list(reviewItems);
		  Assert.assertEquals(5, reviewRecords.size());
		  reviewRecord = reviewRecords.get(0);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{6117767e-8a0b-5624-a599-fa50f96213a6}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(1);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(2);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(3);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(4);
		  Assert.assertEquals(DbUtils.review_status_enum.unreviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{ca3ae25a-164e-27ca-5aef-77a4597c7c32}", reviewRecord.getReviewAgainstItemId());
		  
		  //TODO: finish once #7226 is fixed
		  
		  /*changesetId = Changeset.insertNew(mapId, userId, conn);
		  ReviewTestUtils.changesetId = changesetId;
			changesetDoc = 
	    	(new ChangesetUploadXmlValidator()).parseAndValidate
	    	  (FileUtils.readFileToString(
	          new File(
	            Thread.currentThread().getContextClassLoader().getResource(
	              "hoot/services/review/AllDataTypesReviewChangeset5.osm")
	            .getPath()))
	          .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));
			changesetWriter.write(mapId, changesetId, changesetDoc);
			reviewSynchronizer.updateReviewItems(
				changesetDoc, changesetWriter.getParsedElementIdsToElementsByType());
			
			Assert.assertEquals(
		  	8, 
		  	new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	        .from(elementIdMappings)
	        .where(elementIdMappings.mapId.eq(mapId))
		      .count());
		  reviewRecords =
	      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
			    .from(reviewItems)
			    .where(reviewItems.mapId.eq(mapId))
	  		  .orderBy(reviewItems.reviewId.asc())
	  		  .list(reviewItems);
		  Assert.assertEquals(5, reviewRecords.size());
		  reviewRecord = reviewRecords.get(0);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{6117767e-8a0b-5624-a599-fa50f96213a6}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(1);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(2);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f70c7b9f-a276-5bbc-8149-23c103de2228}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(3);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{fde621f3-c915-5f50-b521-49cb75c20337}", reviewRecord.getReviewAgainstItemId());
		  reviewRecord = reviewRecords.get(4);
		  Assert.assertEquals(DbUtils.review_status_enum.reviewed, reviewRecord.getReviewStatus());
		  Assert.assertEquals(
		  	"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", reviewRecord.getReviewableItemId());
		  Assert.assertEquals(
		  	"{ca3ae25a-164e-27ca-5aef-77a4597c7c32}", reviewRecord.getReviewAgainstItemId());*/
	}
	
  @Category(UnitTest.class)
  public void testUpdateReviewItemsMapDoesntExist() throws Exception
  {
    ReviewItemsSynchronizer reviewItemsSynchronizer = 
    	new ReviewItemsSynchronizer(conn, String.valueOf(mapId + 1));
    Assert.assertFalse(reviewItemsSynchronizer.updateReviewItems(null, null));
  }

  @Category(UnitTest.class)
  public void testUpdateReviewItemsEmptyMapIdParam() throws Exception
  {
  	ReviewItemsSynchronizer reviewItemsSynchronizer = 
    	new ReviewItemsSynchronizer(conn, "");
    Assert.assertFalse(reviewItemsSynchronizer.updateReviewItems(null, null));
  }
	
	@Test
	@Category(UnitTest.class)
	public void testSetAllItemsReviewed() throws Exception
	{
  	ReviewTestUtils.populateReviewDataForAllDataTypes();
  	
  	SetAllItemsReviewedRequest request = new SetAllItemsReviewedRequest();
  	request.setMapId(mapId);
  	/*final String response =*/ 
	  	resource()
	      .path("/review/setallreviewed")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.TEXT_PLAIN)
	      .put(request);
  	Assert.assertEquals(
  		5, 
  		new SQLQuery(conn, DbUtils.getConfiguration(mapId))
        .from(reviewItems)
        .where(
    	    reviewItems.mapId.eq(mapId)
    	      .and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed)))
	      .count());
	}
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testSetAllReviewedMapDoesntExist() throws Exception
  {
    try
    {
    	SetAllItemsReviewedRequest request = new SetAllItemsReviewedRequest();
    	request.setMapId((long)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE));
	  	resource()
	      .path("/review/setallreviewed")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.TEXT_PLAIN)
	      .put(request);
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
  public void testSetAllReviewedMissingMapIdParam() throws Exception
  {
    try
    {
    	SetAllItemsReviewedRequest request = new SetAllItemsReviewedRequest();
	  	resource()
	      .path("/review/setallreviewed")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.TEXT_PLAIN)
	      .put(request);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      //Assert.assertTrue(
        //e.getResponse().getEntity(String.class).contains(""));
      throw e;
    }
  }
}
