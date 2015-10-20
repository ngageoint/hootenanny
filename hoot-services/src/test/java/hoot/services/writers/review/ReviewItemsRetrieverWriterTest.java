package hoot.services.writers.review;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;

import java.sql.Timestamp;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Mockito;

import com.mysema.query.sql.dml.SQLUpdateClause;

public class ReviewItemsRetrieverWriterTest {
	@Test
	@Category(UnitTest.class)
	public void testGetLastAccessUpdateClause() throws Exception
	{
		ReviewItemsRetrieverWriter spy = Mockito.spy(new ReviewItemsRetrieverWriter(null, 1));
	;

		java.util.Date date= new java.util.Date();
  	Timestamp newLastAccessTime = new Timestamp(date.getTime());
		SQLUpdateClause cl = spy._getLastAccessUpdateClause("{123456789}", newLastAccessTime, null);
		String actual = cl.toString();
		
	
		Assert.assertEquals("update \"review_items\"\n" + 
				"set \"last_accessed\" = ?\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"reviewable_item_id\" = ?", actual);
		
		cl = spy._getLastAccessUpdateClause("{123456789}", newLastAccessTime, "{321654987}");
		actual = cl.toString();
		
	
		Assert.assertEquals("update \"review_items\"\n" + 
				"set \"last_accessed\" = ?\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"reviewable_item_id\" = ? "
				+ "and \"review_items\".\"review_against_item_id\" = ?", actual);
	}
	
	
	//_updateLastAccessWithSubSelect
	@Test
	@Category(UnitTest.class)
	public void testUpdateLastAccessWithSubSelect() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsRetrieverWriter spy = Mockito.spy(new ReviewItemsRetrieverWriter(null, 1));
		
		
		java.util.Date date= new java.util.Date();
  	Timestamp now = new Timestamp(date.getTime());

  	SQLUpdateClause q = spy._updateLastAccessWithReviewId(now, 1234);
		
		Assert.assertEquals("update \"review_items\"\n" + 
				"set \"last_accessed\" = ?\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_id\" = ?", q.toString());
	}
}
