package hoot.services.readers.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLQuery;

public class ReviewableBboxQueryTest {
	@Test
  @Category(UnitTest.class)
	public void testGetRelationMembersQuery() throws Exception
	{
		ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);
		
		SQLQuery q = bbxQ._getRelationMembersQuery(101);
		String expected = "from \"current_relation_members_10\" \"current_relation_members\"\n" + 
				"where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?";
		String actual = q.toString();
		org.junit.Assert.assertEquals(expected, actual);
	}
	
	@Test
  @Category(UnitTest.class)
	public void testGetRelationNodeMembersBboxQuery() throws Exception
	{
		ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);
		SQLQuery q = bbxQ._getRelationNodeMembersBboxQuery(102);
		String expected = "from \"current_nodes_10\" \"current_nodes\"\n" + 
				"where \"current_nodes\".\"id\" in (select \"current_relation_members\".\"member_id\"\n" + 
				"from \"current_relation_members_10\" \"current_relation_members\"\n" + 
				"where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?)";
		String actual = q.toString();
		org.junit.Assert.assertEquals(expected, actual);
	}
	
	@Test
  @Category(UnitTest.class)
	public void testGetRelationWayMembersBboxQuery() throws Exception
	{
		ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);
		SQLQuery q = bbxQ._getRelationWayMembersBboxQuery(103);
		String expected = "from \"current_nodes_10\" \"current_nodes\"\n" + 
				"where \"current_nodes\".\"id\" in (select \"current_way_nodes\".\"node_id\"\n" + 
				"from \"current_way_nodes_10\" \"current_way_nodes\"\n" + 
				"where \"current_way_nodes\".\"way_id\" in (select \"current_relation_members\".\"member_id\"\n" + 
				"from \"current_relation_members_10\" \"current_relation_members\"\n" + 
				"where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?))";
		String actual = q.toString();
		org.junit.Assert.assertEquals(expected, actual);
	}
}
