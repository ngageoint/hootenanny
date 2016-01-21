package hoot.services.writers.review;

import java.sql.Connection;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.models.review.ReviewBookmarkDelRequest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.dml.SQLDeleteClause;

public class ReviewBookmarksRemoverTest {
	@Test
  @Category(UnitTest.class)
	public void testDelete() throws Exception
	{
		Connection conn = null;
  	try
  	{
  		ReviewBookmarkDelRequest request = new ReviewBookmarkDelRequest((long)1, (long)2);
  		ReviewBookmarksRemover remover = new ReviewBookmarksRemover(conn);
  		SQLDeleteClause del = remover._createDelClause(request);
  		
  		String actual = del.toString();
  		String expected = "delete from \"review_bookmarks\"\n" + 
  				"where \"review_bookmarks\".\"map_id\" = ? and \"review_bookmarks\".\"relation_id\" = ?";
  		
  		org.junit.Assert.assertEquals(expected, actual);
  	}
    finally
    {
      
    }
  	
		
	}

}
