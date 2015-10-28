/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.readers.review;

import java.sql.Timestamp;

import org.junit.Assert;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.readers.review.ReviewItemsRetriever;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Mockito;

import com.mysema.query.sql.SQLQuery;

public class ReviewItemsRetrieverTest
{
	
	
	@Test
	@Category(UnitTest.class)
	public void testGetAvailableReviewQuery() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		java.util.Date date= new java.util.Date();
  	Timestamp compareTime = new Timestamp(date.getTime());
		SQLQuery q = spy._getAvailableReviewQuery(compareTime, 1, true);
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? and "
				+ "(\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null) "
				+ "or \"review_items\".\"review_id\" = ?\n" + 
				"order by \"review_items\".\"review_id\" asc", q.toString());
		
		
		q = spy._getAvailableReviewQuery(compareTime, 1, false);
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? and "
				+ "(\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null) "
				+ "or \"review_items\".\"review_id\" = ?\n" + 
				"order by \"review_items\".\"review_id\" desc", q.toString());
	}
	
	//testGetAvailableReviewCntQuery
	@Test
	@Category(UnitTest.class)
	public void testGetAvailableReviewCntQuery() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		SQLQuery q = spy.getAvailableReviewCntQuery();
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? and (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null) or \"review_items\".\"review_id\" = ?\n" + 
				"order by \"review_items\".\"review_id\" asc", q.toString());
	}
	
	//getLockedReviewCntQuery()
	@Test
	@Category(UnitTest.class)
	public void testGetLockedReviewCntQuery() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		SQLQuery q = spy.getLockedReviewCntQuery();
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? "
				+ "and \"review_items\".\"last_accessed\" >= ?", q.toString());
	}
	
	//getTotalReviewCntQuery()
	@Test
	@Category(UnitTest.class)
	public void testGetTotalReviewCntQuery() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		SQLQuery q = spy.getTotalReviewCntQuery();
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ?", q.toString());
	}
	
	//_getReviewAgainstForReviewable
	@Test
	@Category(UnitTest.class)
	public void testGetReviewAgainstForReviewable() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		SQLQuery q = spy._getReviewAgainstForReviewable("{123456789}");
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"reviewable_item_id\" = ? "
				+ "and \"review_items\".\"review_status\" = ?\n" + 
				"order by \"review_items\".\"review_id\" asc", q.toString());
	}
	
	//_getElementMappingForReviewable
	@Test
	@Category(UnitTest.class)
	public void testGetElementMappingForReviewable() throws Exception
	{
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		SQLQuery q = spy._getElementMappingForReviewable("{123456789}");
		
		Assert.assertEquals("from \"element_id_mappings\" \"element_id_mappings\"\n" + 
				"where \"element_id_mappings\".\"map_id\" = ? and \"element_id_mappings\".\"element_id\" = ?", q.toString());
	}
	
	//_getAvailableReviewWithOffsetQuery
	@Test
	@Category(UnitTest.class)
	public void testGetAvailableReviewWithOffsetQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
		java.util.Date date= new java.util.Date();
  	Timestamp compareTime = new Timestamp(date.getTime());
		SQLQuery q = spy._getAvailableReviewWithOffsetQuery(compareTime, 123, true);
		
		Assert.assertEquals("from \"review_items\" \"review_items\"\n" + 
				"where \"review_items\".\"map_id\" = ? and \"review_items\".\"review_status\" = ? "
				+ "and (\"review_items\".\"last_accessed\" < ? or \"review_items\".\"last_accessed\" is null)"
				+ " and \"review_items\".\"review_id\" >= ?\n" + 
				"order by \"review_items\".\"review_id\" asc", q.toString());
	}
	

	
	//_getRelationBboxQuery
	@Test
	@Category(UnitTest.class)
	public void testGetRelationBboxQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
  	SQLQuery q = spy._getRelationBboxQuery(1234);
		
		Assert.assertEquals("from \"current_nodes_1\" \"current_nodes\"\n" + 
				"where \"current_nodes\".\"id\" in (select \"current_way_nodes\".\"node_id\"\n" + 
				"from \"current_way_nodes_1\" \"current_way_nodes\"\n" + 
				"where \"current_way_nodes\".\"way_id\" in (select \"current_relation_members\".\"member_id\"\n" + 
				"from \"current_relation_members_1\" \"current_relation_members\"\n" + 
				"where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?)) "
				+ "or \"current_nodes\".\"id\" in (select \"current_relation_members\".\"member_id\"\n" + 
				"from \"current_relation_members_1\" \"current_relation_members\"\n" + 
				"where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?)", q.toString());
	}
	
	//_getWayBboxQuery
	@Test
	@Category(UnitTest.class)
	public void testGetWayBboxQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
  	SQLQuery q = spy._getWayBboxQuery(1234);
		
		Assert.assertEquals("from \"current_nodes_1\" \"current_nodes\"\n" + 
				"where \"current_nodes\".\"id\" in (select \"current_way_nodes\".\"node_id\"\n" + 
				"from \"current_way_nodes_1\" \"current_way_nodes\"\n" + 
				"where \"current_way_nodes\".\"way_id\" = ?)", q.toString());
	}
	
	//_getNodeCoordQuery
	@Test
	@Category(UnitTest.class)
	public void testGetNodeCoordQuery() throws Exception
	{
		DbUtils.createConnection();
		
		ReviewItemsRetriever spy = Mockito.spy(new ReviewItemsRetriever());
		spy.setMapId(1);
		spy.setUserId(1);
		
  	SQLQuery q = spy._getNodeCoordQuery(1234);
		
		Assert.assertEquals("from \"current_nodes_1\" \"current_nodes\"\n" + 
				"where \"current_nodes\".\"id\" = ?", q.toString());
	}
}
