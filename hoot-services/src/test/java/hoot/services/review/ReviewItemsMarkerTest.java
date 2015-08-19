package hoot.services.review;

import java.sql.Timestamp;

import org.junit.Assert;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Mockito;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLUpdateClause;

public class ReviewItemsMarkerTest {
	
	
	@Test
	@Category(UnitTest.class)
	public void testGetLastAccessUpdateClause() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsMarker spy = Mockito.spy(new ReviewItemsMarker());
		spy.setMapId(1);
		spy.setUserId(1);

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
	
	
	@Test
	@Category(UnitTest.class)
	public void testGetAvailableReviewQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsMarker spy = Mockito.spy(new ReviewItemsMarker());
		spy.setMapId(1);
		spy.setUserId(1);
		
		java.util.Date date= new java.util.Date();
  	Timestamp compareTime = new Timestamp(date.getTime());
		SQLQuery q = spy._getAvailableReviewQuery(compareTime);
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? and (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null)\n" + 
				"order by \"review_items\".\"review_score\" desc, \"review_items\".\"review_id\" asc", q.toString());
	}
	
	//_getAvailableReviewWithOffsetQuery
	@Test
	@Category(UnitTest.class)
	public void testGetAvailableReviewWithOffsetQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsMarker spy = Mockito.spy(new ReviewItemsMarker());
		spy.setMapId(1);
		spy.setUserId(1);
		
		java.util.Date date= new java.util.Date();
  	Timestamp compareTime = new Timestamp(date.getTime());
		SQLQuery q = spy._getAvailableReviewWithOffsetQuery(compareTime, "{123456789}", "{a-123456789}");
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and (\"review_items\".\"review_status\" = ? and"
				+ " (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null) "
				+ "or \"review_items\".\"reviewable_item_id\" = ? and \"review_items\".\"review_against_item_id\" = ?)\n" + 
				"order by \"review_items\".\"review_score\" desc, \"review_items\".\"review_id\" asc", q.toString());
	}
	
	
	//_updateLastAccessWithSubSelect
	@Test
	@Category(UnitTest.class)
	public void testUpdateLastAccessWithSubSelect() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsMarker spy = Mockito.spy(new ReviewItemsMarker());
		spy.setMapId(1);
		spy.setUserId(1);
		
		java.util.Date date= new java.util.Date();
  	Timestamp now = new Timestamp(date.getTime());
  	Timestamp compareTime = new Timestamp(date.getTime() - ReviewItemsMarker.LOCK_TIME);
  	SQLUpdateClause q = spy._updateLastAccessWithSubSelect(now, compareTime, "{123456789}", null);
		
		Assert.assertEquals("update \"review_items\"\n" + 
				"set \"last_accessed\" = ?\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_id\" in (select \"review_items\".\"review_id\"\n" + 
				"from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"reviewable_item_id\" = ? and \"review_items\".\"review_status\" != ? "
				+ "and (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null))", q.toString());
		
		q = spy._updateLastAccessWithSubSelect(now, compareTime, "{123456789}", "{321654987}");
		
		Assert.assertEquals("update \"review_items\"\n" + 
				"set \"last_accessed\" = ?\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_id\" in (select \"review_items\".\"review_id\"\n" + 
				"from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"reviewable_item_id\" = ? "
				+ "and \"review_items\".\"review_against_item_id\" = ? and \"review_items\".\"review_status\" != ? "
				+ "and (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null))", q.toString());
	}
	
	
	
}
