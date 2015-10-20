package hoot.services.controllers.job;

import java.io.IOException;
import java.util.Map;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementInfo;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.ReviewRef;
import hoot.services.models.review.ReviewRefsResponse;
import hoot.services.models.review.ReviewRefsRequest;
import hoot.services.models.review.ReviewRefsResponses;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.JsonUtils;
import hoot.services.utils.RandomNumberGenerator;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewResourceGetReferencesTest extends OsmResourceTestAbstract
{
	public ReviewResourceGetReferencesTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
	
	@Test
	@Category(UnitTest.class)
	public void testGetReferences() throws Exception
	{
		ReviewTestUtils testUtils = new ReviewTestUtils();
		/*final long changesetId =*/ testUtils.populateReviewDataForAllDataTypes(mapId, userId);
		final Map<Long, Element> oldNodeIdsToNewNodes = 
			testUtils.parsedElementIdsToElementsByType.get(ElementType.Node);
		
		final ReviewRefsResponses response = 
	  	resource()
	      .path("/review/refs")
	      .queryParam(
        	"queryElements", 
          JsonUtils.objectToJson(
          	new ReviewRefsRequest(
      		    new ElementInfo[] { 
      			    new ElementInfo(
      			    	String.valueOf(mapId), oldNodeIdsToNewNodes.get((long)-116).getId(), "node"),
      			    new ElementInfo(
      			    	String.valueOf(mapId), oldNodeIdsToNewNodes.get((long)-117).getId(), "node")
      			  })))
	      .accept(MediaType.APPLICATION_JSON)
        .get(ReviewRefsResponses.class);
	  
		final ReviewRefsResponse[] reviewReferenceResponses = response.getReviewRefResponses();
		Assert.assertEquals(2, reviewReferenceResponses.length);
		for (int i = 0; i < reviewReferenceResponses.length; i++)
		{
			ReviewRefsResponse refsResponse = reviewReferenceResponses[i];
			ElementInfo queryElementInfo = refsResponse.getQueryElementInfo();
			
			if (i == 0)
			{
				Assert.assertEquals(
					oldNodeIdsToNewNodes.get((long)-116).getId(), queryElementInfo.getElementId());
				Assert.assertEquals("node", queryElementInfo.getElementType());
				
				final ReviewRef[] refs = refsResponse.getReviewRefs();
				Assert.assertEquals(2, refs.length);
				
				Assert.assertEquals(oldNodeIdsToNewNodes.get((long)-46).getId(), refs[0].getElementId());
				Assert.assertEquals("node", refs[0].getElementType());
				Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
				Assert.assertEquals(2, refs[0].getReviewRelationId());
				
				Assert.assertEquals(oldNodeIdsToNewNodes.get((long)-49).getId(), refs[1].getElementId());
				Assert.assertEquals("node", refs[1].getElementType());
				Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
				Assert.assertEquals(3, refs[1].getReviewRelationId());
			}
			else if (i == 1)
			{
				Assert.assertEquals(
					oldNodeIdsToNewNodes.get((long)-117).getId(), queryElementInfo.getElementId());
				Assert.assertEquals("node", queryElementInfo.getElementType());
				
				final ReviewRef[] refs = refsResponse.getReviewRefs();
				Assert.assertEquals(2, refs.length);
				
				Assert.assertEquals(oldNodeIdsToNewNodes.get((long)-42).getId(), refs[0].getElementId());
				Assert.assertEquals("node", refs[0].getElementType());
				Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
				Assert.assertEquals(4, refs[0].getReviewRelationId());
				
				Assert.assertEquals(oldNodeIdsToNewNodes.get((long)-47).getId(), refs[1].getElementId());
				Assert.assertEquals("node", refs[1].getElementType());
				Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
				Assert.assertEquals(6, refs[1].getReviewRelationId());
			}
		}
	}
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam(
        	"queryElements", 
          JsonUtils.objectToJson(
          	new ReviewRefsRequest(
      		    new ElementInfo[] { 
      			    new ElementInfo(
      				    String.valueOf(
      				    	(long)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE)), 
      				    -1, 
      				    "")})))
        .type(MediaType.TEXT_PLAIN)
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewRefsResponses.class);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      //System.out.println(e.getResponse().getEntity(String.class));
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("No map exists"));

      throw e;
    }
  }
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetQueryElementDoesntExist() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam(
        	"queryElements", 
          JsonUtils.objectToJson(
          	new ReviewRefsRequest(
      		    new ElementInfo[] { new ElementInfo(String.valueOf(mapId), -1, "node")})))
        .type(MediaType.TEXT_PLAIN)
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewRefsResponses.class);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      //System.out.println(e.getResponse().getEntity(String.class));
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("does not exist"));

      throw e;
    }
  }
  
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetQueryElementEmptyElementType() throws Exception
  {
    try
    {
    	resource()
        .path("/review/refs")
        .queryParam(
        	"queryElements", 
          JsonUtils.objectToJson(
          	new ReviewRefsRequest(
      		    new ElementInfo[] { new ElementInfo(String.valueOf(mapId), -1, "")})))
        .type(MediaType.TEXT_PLAIN)
        .accept(MediaType.APPLICATION_JSON)
        .get(ReviewRefsResponses.class);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      //System.out.println(e.getResponse().getEntity(String.class));
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("does not exist"));

      throw e;
    }
  }
}
