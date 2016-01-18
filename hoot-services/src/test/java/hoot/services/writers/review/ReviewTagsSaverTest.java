package hoot.services.writers.review;

import java.sql.Connection;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.models.review.ReviewTagSaveRequest;

import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ReviewTagsSaverTest {

	@Test
  @Category(UnitTest.class)
	public void testInsert() throws Exception
	{
		Connection conn = DbUtils.createConnection();
  	try
  	{
  		ReviewTagSaveRequest req = new ReviewTagSaveRequest(1, 1, null, -1);
  		ReviewTagsSaver tagsSaver = new ReviewTagsSaver(conn);
  		tagsSaver.insert(req);
  	}
    finally
    {
      DbUtils.closeConnection(conn);
    }
  	
		
	}
	
	
	@Test
  @Category(UnitTest.class)
	public void testInsertWithDetail() throws Exception
	{
		Connection conn = DbUtils.createConnection();
  	try
  	{
  		JSONObject o = new JSONObject();
  		o.put("test1", "val1");
  		o.put("test2", "val2");
  		ReviewTagSaveRequest req = new ReviewTagSaveRequest(1, 1, o, -1);
  		ReviewTagsSaver tagsSaver = new ReviewTagsSaver(conn);
  		tagsSaver.insert(req);
  	}
    finally
    {
      DbUtils.closeConnection(conn);
    }
  	
		
	}
}
