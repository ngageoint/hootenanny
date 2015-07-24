package hoot.services.readers.review;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QReviewItems;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.support.Expressions;
import com.mysema.query.types.expr.BooleanExpression;
import com.mysema.query.types.path.SimplePath;
import com.mysema.query.types.query.ListSubQuery;

public class ReviewableItemsReaderTest {
	protected static final QReviewItems reviewItemsTbl = QReviewItems.reviewItems;
	protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
	protected static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
	protected static final QCurrentWays currentWays = QCurrentWays.currentWays;
	protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
	protected static final QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
	protected static final QReviewItems reviewItems = QReviewItems.reviewItems;


	@Test
	@Category(UnitTest.class)
	public void testGetWayBoundingBox() throws Exception
	{
		/*Connection conn = DbUtils.createConnection();
		
  	final SimplePath<String> cwn = Expressions.path(String.class, "cwn");
  	final SimplePath<Long> cwn_wayid = Expressions.path(Long.class, "cwn.wayid");
  	final SimplePath<Long> cwn_nodeid = Expressions.path(Long.class, "cwn.nodeid");
  	
  	final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	
  	final SimplePath<String> reviewRelJoin = Expressions.path(String.class, "review_rel_id_join_rel_members");
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids");
  	
  	final SimplePath<String> cr_cn = Expressions.path(String.class, "cr_cn");
  	final SimplePath<String> rel_rel = Expressions.path(String.class, "rel_rel");
  	// select node from relation_members table
  	//	add x,y as min xy and max xy to store
  	final ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) 4))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));
  	final ListSubQuery<Tuple> relationMembers = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.where(currentRelationMembers.memberType.eq("node"))
  			.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);

  	
  	
  	SQLQuery bboxQuery = new SQLQuery(conn, DbUtils.getConfiguration(4)).from(currentNodes).
  			join(relationMembers, cr_cn).on(currentNodes.id.eq(rel_mem_ids))
  			.groupBy(review_rel_id);
  	
  	String res = bboxQuery.toString();
  	

  	SQLSubQuery q = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id));

  	
  	ListSubQuery<Tuple> relRelation = q
  			.where(currentRelationMembers.memberType.eq("relation"))
  			.list(currentRelationMembers.memberId.as(rel_mem_ids), review_rel_id);
  	
  	SQLSubQuery qq = new SQLSubQuery().from(currentRelationMembers).
  			join(relRelation,rel_rel).on(currentRelationMembers.relationId.eq(rel_mem_ids));
  	
  	String xx = qq.toString();	*/
  /*	ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) 4))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));
  	SQLSubQuery q = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id));
  	
  	BooleanExpression bb = currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way);
  	ListSubQuery<Tuple> relationWayMembers = q
  			.where(bb)
  			.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);*/
  	
 /* 	ReviewableItemsReader read = new ReviewableItemsReader(conn,4);
  	
  	ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) 4))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));


  	
  	ListSubQuery<Tuple> relationIds = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation))
    		.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);
  	
  	List<Tuple> res2 = read._getRelWay(relationIds,1);/*
  	
  	
  	
  	
	}
	@Test
	@Category(UnitTest.class)
	public void testGetReviewItems() throws Exception
	{
		Connection conn = DbUtils.createConnection();
		ReviewableItemsReader read = new ReviewableItemsReader(conn,4);
		read.getReviewItems(0, false, 0);
		// id=member_id of map id of with sequence_id=0
		// type=member_type
		// reviewScore=review_score
		// displayBounds=_getRelationsBoundingBox() and search member_id
		// itemToReviewAgainst= 
		//      id=member_id of map id of with sequence_id>0
		//      type=member_type of squence_id>0
/*		
		final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
		final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
		
		final SimplePath<Double> review_score = Expressions.path(Double.class, "review_score");
		
		final ListSubQuery<Tuple> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) 4)
				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id), reviewItems.reviewScore.as(review_score));
		
		
		
		List<Tuple> res = new SQLQuery(conn, DbUtils.getConfiguration(4)).from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.list(currentRelationMembers.relationId, currentRelationMembers.memberId,currentRelationMembers.memberType,currentRelationMembers.sequenceId, review_score);
		
		for(int i=0; i<res.size(); i++)
		{
			Tuple tup = res.get(i);
			
			Long relId = tup.get(currentRelationMembers.relationId);
			Long memId = tup.get(currentRelationMembers.memberId);
			String memType = tup.get(currentRelationMembers.memberType).toString();
			Double revScore = tup.get(review_score);
			Integer seq = tup.get(currentRelationMembers.sequenceId);
			
			if(seq == 0)
			{
				// primary
			}
			else
			{
				// review against
			}
			
		}*/
		
	}
}
