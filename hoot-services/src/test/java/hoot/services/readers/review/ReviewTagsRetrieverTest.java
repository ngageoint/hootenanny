package hoot.services.readers.review;

import hoot.services.UnitTest;

import java.sql.Connection;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLQuery;

public class ReviewTagsRetrieverTest {
	
	@Test
  @Category(UnitTest.class)
	public void testRetrieve() throws Exception
	{
		Connection conn = null;
  	
		ReviewTagRetriever r = new  ReviewTagRetriever(conn);
		SQLQuery q = r._getQuery(1, 2);
		
		String actual = q.toString();
		String expected = "from \"review_tags\" \"review_tags\"\n" + 
				"where \"review_tags\".\"map_id\" = ? and \"review_tags\".\"relation_id\" = ?";
		
		org.junit.Assert.assertEquals(expected, actual);
  	
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testRetrieveAllOrderByCreatedAt() throws Exception
	{
		Connection conn = null;
  	
		ReviewTagRetriever r = new  ReviewTagRetriever(conn);
		SQLQuery q = r._getAllQuery("createdAt", true, -1, -1);
		
		String actual = q.toString();
		String expected = "from \"review_tags\" \"review_tags\"\n" + 
				"order by \"review_tags\".\"created_at\" asc";
		
		org.junit.Assert.assertEquals(expected, actual);
  	
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testRetrieveAllOrderByCreatedAtDesc() throws Exception
	{
		Connection conn = null;
  	
		ReviewTagRetriever r = new  ReviewTagRetriever(conn);
		SQLQuery q = r._getAllQuery("createdAt", false, -1, -1);
		
		String actual = q.toString();
		String expected = "from \"review_tags\" \"review_tags\"\n" + 
				"order by \"review_tags\".\"created_at\" desc";
		
		org.junit.Assert.assertEquals(expected, actual);
  	
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testRetrieveAllOrderByIdLimit100() throws Exception
	{
		Connection conn = null;
  	
		ReviewTagRetriever r = new  ReviewTagRetriever(conn);
		SQLQuery q = r._getAllQuery("id", true, 100, -1);
		
		String actual = q.toString();
		String expected = "from \"review_tags\" \"review_tags\"\n" + 
				"order by \"review_tags\".\"id\" asc\n" + 
				"limit ?";
		
		org.junit.Assert.assertEquals(expected, actual);
  	
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testRetrieveAllOrderByIdLimit100Offset123() throws Exception
	{
		Connection conn = null;
  	
		ReviewTagRetriever r = new  ReviewTagRetriever(conn);
		SQLQuery q = r._getAllQuery("id", true, 100, 123);
		
		String actual = q.toString();
		String expected = "from \"review_tags\" \"review_tags\"\n" + 
				"order by \"review_tags\".\"id\" asc\n" + 
				"limit ?\n" + 
				"offset ?";
		
		org.junit.Assert.assertEquals(expected, actual);
  	
	}

}
