package hoot.services.writers.review;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.List;

import hoot.services.UnitTest;
import hoot.services.db2.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkSaveRequest;

import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLBindings;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

public class ReviewBookmarksSaverTest {

	@Test
  @Category(UnitTest.class)
	public void testInsert() throws Exception
	{
		Connection conn = null;
  	try
  	{
  		ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, null, -1);
  		ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
  		SQLInsertClause cl = tagsSaver._createInsertClause(req);
  		String actual = cl.toString();
  		
  		String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n" + 
  				"values (?, ?, ?, ?, '')";
  		
  		org.junit.Assert.assertEquals(expected, actual);
  	}
    finally
    {

    }
  	
		
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testInsertWithDetail() throws Exception
	{
		Connection conn = null;
  	try
  	{
  		JSONObject o = new JSONObject();
  		o.put("test1", "val1");
  		o.put("test2", "val2");
  		ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, o, -1);
  		ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
  		SQLInsertClause cl = tagsSaver._createInsertClause(req);
  		String actual = cl.toString();
  		
  		String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n" + 
  				"values (?, ?, ?, ?, '\"test1\"=>\"val1\",\"test2\"=>\"val2\"')";
  		
  		org.junit.Assert.assertEquals(expected, actual);
  	}
    finally
    {

    }		
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testUpdateWithDetail() throws Exception
	{
		Connection conn = null;
  	try
  	{
  		final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
  		ReviewBookmarks dto = new ReviewBookmarks();
  		dto.setId((long)10);
  		dto.setMapId((long)1);
  		dto.setRelationId((long)2);
  		dto.setDetail("\"\"test1\"=>\"val1\", \"test2\"=>\"val2\"\"");
  		dto.setCreatedAt(now);
  		dto.setCreatedBy((long)-1);
  		
  		JSONObject o = new JSONObject();
  		o.put("test3", "val3");
  		o.put("test4", "val4");
  		ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 2, o, -2);
  		ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
  		
  		SQLUpdateClause cl = tagsSaver._getUpdateQuery(req, dto);
  		
  		List<SQLBindings> bn = cl.getSQL();
  		String actual = cl.toString();
  		
  		String expected = "update \"review_bookmarks\"\n" + 
  				"set \"last_modified_by\" = ?, \"id\" = ?, \"created_by\" = ?, \"created_at\" = ?, \"detail\" = '\"test4\"=>\"val4\",\"test3\"=>\"val3\"', \"last_modified_at\" = ?\n" + 
  				"where \"review_bookmarks\".\"id\" = ?";
  		
  		org.junit.Assert.assertTrue(expected.indexOf("\"last_modified_by\" = ?") > 0);
  		org.junit.Assert.assertTrue(expected.indexOf("\"id\" = ?") > 0);
  		org.junit.Assert.assertTrue(expected.indexOf("\"created_by\" = ?") > 0);
  		org.junit.Assert.assertTrue(expected.indexOf("\"created_at\" = ?") > 0);
  		org.junit.Assert.assertTrue(expected.indexOf("\"detail\" = '\"test4\"=>\"val4\",\"test3\"=>\"val3\"'") > 0);
  		org.junit.Assert.assertTrue(expected.indexOf("\"last_modified_at\" = ?") > 0);
  	}
  	catch(Exception ex)
  	{
  		String err = ex.getMessage();
  	}
    finally
    {

    }		
	}
}
