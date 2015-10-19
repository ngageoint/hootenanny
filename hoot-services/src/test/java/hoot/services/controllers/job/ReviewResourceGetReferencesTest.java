package hoot.services.controllers.job;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db2.QCurrentRelations;
import hoot.services.models.review.ReviewReferencesCollection;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewResourceGetReferencesTest extends OsmResourceTestAbstract
{
	protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
	
	public ReviewResourceGetReferencesTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
	
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testGetReferences() throws Exception
	{
		/*ReviewTestUtils.populateReviewDataForAllDataTypes();
		
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
	  Assert.assertEquals("{c254d8ab-3f1a-539f-91b7-98b485c5c129}", firstReviewable2.getUuid());
	  final ReviewAgainstItem secondReviewable2 = reviewables2[1];
	  Assert.assertEquals("node", secondReviewable2.getType().toLowerCase());
	  Assert.assertEquals("{d1012bc9-92bc-5931-aac2-aa5702f42b8b}", secondReviewable2.getUuid());
	  Assert.assertNull(refs2.getReviewAgainstItems());*/
	}
	
	@Ignore
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam(
        	"mapId", 
        	String.valueOf((int)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE)))
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

	@Ignore
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

  @Ignore
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
  
  @Ignore
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetElementUniqueIdDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam(
        	"mapId", 
        	String.valueOf((int)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE)))
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
  
  @Ignore
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMissingElementUniqueIdsParam() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId))
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

  @Ignore
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyElementUniqueIdsParam() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam("mapId", String.valueOf(mapId))
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
