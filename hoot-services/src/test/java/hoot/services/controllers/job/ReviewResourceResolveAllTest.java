package hoot.services.controllers.job;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.db2.QCurrentRelations;
import hoot.services.models.review.MapReviewResolverRequest;
import hoot.services.models.review.MapReviewResolverResponse;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;

import org.junit.Assert;
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
	
	@Test
	@Category(UnitTest.class)
	public void testSetAllReviewsResolved() throws Exception
	{
  	/*final long changesetId =*/ ReviewTestUtils.populateReviewDataForAllDataTypes(mapId, userId);
  	
  	final MapReviewResolverResponse response = 
  		resource()
        .path("/review/resolveall")
        .type(MediaType.APPLICATION_JSON)
        .accept(MediaType.APPLICATION_JSON)
        .put(MapReviewResolverResponse.class, new MapReviewResolverRequest(String.valueOf(mapId)));
  	
  	Statement stmt = null;
  	ResultSet rs = null;
  	
    //all review relations should have been set to resolved
  	String sql = "select count(*) from current_relations_" + mapId;
  	sql += " where tags->'hoot:review:needs' = 'yes'";
  	try
  	{
  		stmt = conn.createStatement();
  		rs = stmt.executeQuery(sql);
  		rs.next();
  		
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
  	
    //all review relations should have the incremented changeset id
    sql = "select count(*) from current_relations_" + mapId;
    //TODO: don't think this is right...
  	sql += " where tags->'type' = 'review' and tags->'hoot:review:needs' = 'no' and " +
      "changeset_id = " + response.getChangesetId();
  	try
  	{
  		stmt = conn.createStatement();
  		rs = stmt.executeQuery(sql);
  		rs.next();
  		Assert.assertEquals(5, rs.getInt(1));
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
  	
    //all review relations should have had their version incremented by one
  	sql = "select count(*) from current_relations_" + mapId;
  	sql += " where tags->'type' = 'review' and tags->'hoot:review:needs' = 'no' and version = 2";
  	try
  	{
  		stmt = conn.createStatement();
  		rs = stmt.executeQuery(sql);
  		rs.next();
  		Assert.assertEquals(5, rs.getInt(1));
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
	}
	
	@Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testSetAllReviewsResolvedMapDoesntExist() throws Exception
  {
    try
    {
	  	resource()
	      .path("/review/resolveall")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.APPLICATION_JSON)
	      .put(
	      	new MapReviewResolverRequest(
	      		String.valueOf((long)RandomNumberGenerator.nextDouble(mapId + 10^4, Integer.MAX_VALUE))));
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
	  	resource()
	      .path("/review/resolveall")
	      .type(MediaType.APPLICATION_JSON)
	      .accept(MediaType.APPLICATION_JSON)
	      .put(new MapReviewResolverRequest());
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
