package hoot.services.readers.review;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewItems;
import hoot.services.db2.ReviewMap;
import hoot.services.job.JobStatusManager;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewReferences;
import hoot.services.models.review.ReviewReferencesCollection;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewUtils;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.dml.SQLInsertClause;
import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewResourceGetReferencesTest extends OsmResourceTestAbstract
{
	private static final QReviewMap reviewMap = QReviewMap.reviewMap;
	private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
	
	public ReviewResourceGetReferencesTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
	
	private void populateReviewData() throws Exception
	{
		final String fakeJobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(fakeJobId, JobStatusManager.JOB_STATUS.COMPLETE.toInt(), conn);
    ReviewMap mapReviewInfoRecord = new ReviewMap();
		mapReviewInfoRecord.setMapId(mapId);
		mapReviewInfoRecord.setReviewPrepareJobId(fakeJobId);
		new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
		  .populate(mapReviewInfoRecord)
		  .execute();
		
		List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{f70c7b9f-a276-5bbc-8149-23c103de2228}", 
				1, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{fde621f3-c915-5f50-b521-49cb75c20337}", 
				2, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", 
				3, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", 
				4, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{6117767e-8a0b-5624-a599-fa50f96213a6}", 
				5, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", 
				6, 
				ElementType.Node, 
				mapId));
		elementIdMappingRecordsToInsert.add(
			ReviewUtils.createElementIdMappingRecord(
				"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", 
				7, 
				ElementType.Node, 
				mapId));
		DbUtils.batchRecords(
  	  mapId, elementIdMappingRecordsToInsert, elementIdMappings, null, RecordBatchType.INSERT, conn, 
  	  100);
		
		List<ReviewItems> reviewItemRecordsToInsert = new ArrayList<ReviewItems>();
		reviewItemRecordsToInsert.add(
  		ReviewUtils.createReviewItemRecord(
  			"{f70c7b9f-a276-5bbc-8149-23c103de2228}", 
  			1.0,
  			"{fde621f3-c915-5f50-b521-49cb75c20337}", 
  			mapId));
		reviewItemRecordsToInsert.add(
  		ReviewUtils.createReviewItemRecord(
  			"{f70c7b9f-a276-5bbc-8149-23c103de2228}", 
  			1.0,
  			"{db8f0c0e-35c2-5657-ac30-811e2c1554de}", 
  			mapId));
		reviewItemRecordsToInsert.add(
  		ReviewUtils.createReviewItemRecord(
  			"{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", 
  			1.0,
  			"{6117767e-8a0b-5624-a599-fa50f96213a6}", 
  			mapId));
		reviewItemRecordsToInsert.add(
  		ReviewUtils.createReviewItemRecord(
  			"{f6224e55-cfa6-5364-bb78-e482e2d6e1c5}", 
  			1.0,
  			"{fde621f3-c915-5f50-b521-49cb75c20337}", 
  			mapId));
		reviewItemRecordsToInsert.add(
  		ReviewUtils.createReviewItemRecord(
  			"{c254d8ab-3f1a-539f-91b7-98b485c5c129}", 
  			1.0,
  			"{6117767e-8a0b-5624-a599-fa50f96213a6}", 
  			mapId));
  	DbUtils.batchRecords(
  	  mapId, reviewItemRecordsToInsert, reviewItems, null, RecordBatchType.INSERT, conn, 100);
	}
	
	@Test
	@Category(UnitTest.class)
	public void testGetReferences() throws Exception
	{
		populateReviewData();
		
		final ReviewReferencesCollection response = 
	  	resource()
	      .path("/review/refs")
	      .queryParam("mapId", String.valueOf(mapId))
	      .queryParam(
	      	"elementUniqueIds", 
	      	"{c254d8ab-3f1a-539f-91b7-98b485c5c129};{6117767e-8a0b-5624-a599-fa50f96213a6}")
	      .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
	  
		final ReviewReferences[] refs = response.getReviewReferences();
	  Assert.assertEquals(2, refs.length);
	  
	  final ReviewReferences refs1 = refs[0];
	  Assert.assertEquals("{c254d8ab-3f1a-539f-91b7-98b485c5c129}", refs1.getUniqueId());
	  Assert.assertNull(refs1.getReviewableItems());
	  final ReviewAgainstItem[] reviewAgainst1 = refs1.getReviewAgainstItems();
	  Assert.assertEquals(1, reviewAgainst1.length);
	  final ReviewAgainstItem firstReviewAgainst1 = reviewAgainst1[0];
	  Assert.assertEquals("node", firstReviewAgainst1.getType().toLowerCase());
	  Assert.assertEquals("{6117767e-8a0b-5624-a599-fa50f96213a6}", firstReviewAgainst1.getUuid());
	  
	  final ReviewReferences refs2 = refs[1];
	  Assert.assertEquals("{6117767e-8a0b-5624-a599-fa50f96213a6}", refs2.getUniqueId());
	  final ReviewAgainstItem[] reviewables2 = refs2.getReviewableItems();
	  Assert.assertEquals(2, reviewables2.length);
	  final ReviewAgainstItem firstReviewable2 = reviewables2[0];
	  Assert.assertEquals("node", firstReviewable2.getType().toLowerCase());
	  Assert.assertEquals("{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", firstReviewable2.getUuid());
	  final ReviewAgainstItem secondReviewable2 = reviewables2[1];
	  Assert.assertEquals("node", secondReviewable2.getType().toLowerCase());
	  Assert.assertEquals("{c254d8ab-3f1a-539f-91b7-98b485c5c129}", secondReviewable2.getUuid());
	  Assert.assertNull(refs2.getReviewAgainstItems());
	}
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId + 1))
        .queryParam(
      	  "elementUniqueIds", 
      	  "{c254d8ab-3f1a-539f-91b7-98b485c5c129};{6117767e-8a0b-5624-a599-fa50f96213a6}")
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
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
    	resource()
        .path("/review/refs")
        .queryParam(
    	    "elementUniqueIds", 
    	    "{c254d8ab-3f1a-539f-91b7-98b485c5c129};{6117767e-8a0b-5624-a599-fa50f96213a6}")
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid input parameter value"));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyMapIdParam() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", "")
        .queryParam(
    	    "elementUniqueIds", 
    	    "{c254d8ab-3f1a-539f-91b7-98b485c5c129};{6117767e-8a0b-5624-a599-fa50f96213a6}")
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
    }
    catch (UniformInterfaceException e)
    {
    	Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid input parameter value"));
      throw e;
    }
  }
  
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetElementUniqueIdDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId + 1))
        .queryParam(
    	    "elementUniqueIds", 
    	    //invalid uuid
    	    "{d254d8ab-3f1a-539f-91b7-98b485c5c129}")
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
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
  public void testGetMissingElementUniqueIdsParam() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId + 1))
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
    }
    catch (UniformInterfaceException e)
    {
    	Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid input parameter value"));
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyElementUniqueIdsParam() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId + 1))
        .queryParam("elementUniqueIds", "")
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewReferencesCollection.class);
    }
    catch (UniformInterfaceException e)
    {
    	Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("Invalid input parameter value"));
      throw e;
    }
  }
}
