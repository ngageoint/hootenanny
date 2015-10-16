package hoot.services.writers.review;

import java.io.IOException;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.models.review.SetAllItemsReviewedRequest;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.utils.RandomNumberGenerator;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewStatusModifierTest extends OsmResourceTestAbstract
{
	public ReviewStatusModifierTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
		
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testSetAllItemsReviewed() throws Exception
	{
  	//ReviewTestUtils.populateReviewDataForAllDataTypes();
  	
  	SetAllItemsReviewedRequest request = new SetAllItemsReviewedRequest();
  	request.setMapId(mapId);
  	resource()
      .path("/review/setallreviewed")
      .type(MediaType.APPLICATION_JSON)
      .accept(MediaType.TEXT_PLAIN)
      .put(request);
  	/*Assert.assertEquals(
  		5, 
  		new SQLQuery(conn, DbUtils.getConfiguration(mapId))
        .from(reviewItems)
        .where(
    	    reviewItems.mapId.eq(mapId)
    	      .and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.reviewed)))
	      .count());*/
	}
	
	@Ignore
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
	      .put((long)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE));
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
