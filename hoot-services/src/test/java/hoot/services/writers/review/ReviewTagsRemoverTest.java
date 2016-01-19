package hoot.services.writers.review;

import java.sql.Connection;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.models.review.ReviewTagDelRequest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.dml.SQLDeleteClause;

public class ReviewTagsRemoverTest {
	@Test
  @Category(UnitTest.class)
	public void testDelete() throws Exception
	{
		Connection conn = null;
  	try
  	{
  		ReviewTagDelRequest request = new ReviewTagDelRequest((long)1, (long)2);
  		ReviewTagsRemover remover = new ReviewTagsRemover(conn);
  		SQLDeleteClause del = remover._createDelClause(request);
  		
  		String actual = del.toString();
  		String expected = "delete from \"review_tags\"\n" + 
  				"where \"review_tags\".\"map_id\" = ? and \"review_tags\".\"relation_id\" = ?";
  		
  		org.junit.Assert.assertEquals(expected, actual);
  	}
    finally
    {
      
    }
  	
		
	}

}
