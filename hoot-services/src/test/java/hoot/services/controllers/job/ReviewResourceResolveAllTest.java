package hoot.services.controllers.job;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db2.QCurrentRelations;
import hoot.services.models.review.MapReviewResolverRequest;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.sun.jersey.api.client.UniformInterfaceException;

public class ReviewResourceResolveAllTest extends OsmResourceTestAbstract
{
	protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
	
	public ReviewResourceResolveAllTest() throws NumberFormatException, IOException
  {
    super(new String[]{ "hoot.services.controllers.job" });
  }
		
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testSetAllReviewsResolved() throws Exception
	{
  	ReviewTestUtils.populateReviewDataForAllDataTypes();
  	
  	MapReviewResolverRequest request = new MapReviewResolverRequest();
  	request.setMapId(String.valueOf(mapId));
  	resource()
      .path("/review/resolveall")
      .type(MediaType.APPLICATION_JSON)
      .accept(MediaType.TEXT_PLAIN)
      .put(request);
  	
  	Statement stmt = null;
  	ResultSet rs = null;
  	
  	String sql = "select count(*) from current_relations_" + mapId;
  	sql += " where tags->'hoot:review:needs' = 'yes'";
  	try
  	{
  		stmt = conn.createStatement();
  		rs = stmt.executeQuery(sql);
  		rs.next();
  		//should be no items left to review
  		Assert.assertEquals(0, rs.getInt(1));
  	}
  	finally
  	{
  		if (rs != null)
  		{
  			rs.close();
  		}
  		if (stmt != null)
  		{
  			stmt.close();
  		}
  	}
  	
  	//TODO: finish
  	
  	//verify changeset ID was updated
  	
    sql = "select count(*) from current_relations_" + mapId;
  	sql += " where tags->'type' = 'review' and changeset_id != ";
  	
  	//verify version was incremented
  	
  	
	}
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testSetAllReviewsResolvedMapDoesntExist() throws Exception
  {
    try
    {
    	MapReviewResolverRequest request = new MapReviewResolverRequest();
    	request.setMapId(
    		String.valueOf((long)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE)));
	  	resource()
	      .path("/review/resolveall")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.TEXT_PLAIN)
	      .put(request);
    }
    catch (UniformInterfaceException e)
    {
      Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
      Assert.assertTrue(
        e.getResponse().getEntity(String.class).contains("No map exists"));

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testSetAllReviewsResolvedMissingMapIdParam() throws Exception
  {
    try
    {
    	MapReviewResolverRequest request = new MapReviewResolverRequest();
	  	resource()
	      .path("/review/resolveall")
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
