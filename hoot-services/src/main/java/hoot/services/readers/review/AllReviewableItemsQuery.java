package hoot.services.readers.review;

import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItemBbox;
import hoot.services.models.review.ReviewableItemBboxInfo;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

public class AllReviewableItemsQuery extends ReviewableQueryBase implements
    IReviewableQuery {

	private BoundingBox _bbox;
	private static final Logger log = LoggerFactory.getLogger(AllReviewableItemsQuery.class);
	
	public AllReviewableItemsQuery(final Connection c, final long mapid, final BoundingBox bbox)
	{
		super(c, mapid);
		_bbox = bbox;
	}
	
	
	@Override
	public ReviewQueryMapper execQuery() throws SQLException, Exception {
		
		AllReviewableItems ret = new AllReviewableItems(getMapId(), new HashMap<Long, ReviewableItemBboxInfo>());
		try
		{
			Map<Long, ReviewableItemBboxInfo> allReviewables = new HashMap<>();
			long nNodeCnt = _getReviewableRelationMembersCount("node");
			long nWayCnt = _getReviewableRelationMembersCount("way");
			long nRelationCnt = _getReviewableRelationMembersCount("relation");
			
			// Step1: filter down to element type. Don't bother to run query on non existent element type
			if(nNodeCnt > 0)
			{
				Map<Long, ReviewableItemBboxInfo> reviewRelationWithNodeMembers =  _getReviewableRelatioWithNodeMembersCentroidInBbox(); 
				_combineBbox( allReviewables, reviewRelationWithNodeMembers);
							
			}
			
			if(nWayCnt > 0)
			{
				Map<Long, ReviewableItemBboxInfo> reviewRelationWithWayMembers =  _getReviewableRelatioWithWayMembersCentroidInBbox(); 
				_combineBbox( allReviewables, reviewRelationWithWayMembers);
			}
			
			if(nRelationCnt > 0)
			{
				Map<Long, BoundingBox> relsBbox = new HashMap<>();
				List<JSONObject> rels = _getReviewableRelationMembers();
				for(JSONObject rel : rels)
				{
					long relId = (long) rel.get("memberid");
					if(!relsBbox.containsKey(relId))
					{					
						ReviewableItemBbox bbxInfo = _getRelationMemberBbox(relId);
						if(bbxInfo != null)
						{
							relsBbox.put(relId, bbxInfo.getBbox());
						}
					}
				}
				Map<Long, ReviewableItemBboxInfo> reviewRelationWithRelationMembers  = new HashMap<>();
				for(JSONObject rel : rels)
				{
					long relId = (long) rel.get("relationid");
					long memId = (long) rel.get("memberid");
					String needReview = (String)rel.get("needreview");
					
					BoundingBox memBbox = relsBbox.get(memId);
					// we have relation member bbox and it is within the requested bbox
					// so if the relation member bbox exists in map (meaning relation has multiple relation members)
					// then expand the bbox else just add to the map
					if(memBbox != null && _bbox.intersects(memBbox))
					{
						ReviewableItemBboxInfo relBboxInfo = reviewRelationWithRelationMembers.get(relId);
						
						if(relBboxInfo != null)
						{
							BoundingBox relBbox = relBboxInfo.getBbox();
							relBbox.add(memBbox);
						}
						else
						{
							ReviewableItemBboxInfo newInfo = new ReviewableItemBboxInfo(memBbox, getMapId(), relId,needReview);
							reviewRelationWithRelationMembers.put(relId, newInfo);
						}
						
						
					}
				}
				
				_combineBbox( allReviewables, reviewRelationWithRelationMembers);
					
				
			}
			
			ret = new AllReviewableItems(getMapId(), allReviewables);
			
			
			
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return ret;
	}
	
	protected ReviewableItemBbox _getRelationMemberBbox(final long relId) throws Exception
	{
		ReviewableItemBbox ret = null;
		try
		{
			ReviewableBboxQuery bbq = new ReviewableBboxQuery(getConnection(), getMapId(), relId);
			ret = (ReviewableItemBbox) bbq.execQuery();
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return ret;
	}

	
	private void _combineBbox(final Map<Long, ReviewableItemBboxInfo> allReviewables, 
			final Map<Long, ReviewableItemBboxInfo> reviewRelationWithMembers) throws Exception
	{
		try
		{
		Iterator it = reviewRelationWithMembers.entrySet().iterator();
    while (it.hasNext()) {
        Map.Entry pair = (Map.Entry)it.next();
        ReviewableItemBboxInfo currInfo = (ReviewableItemBboxInfo)pair.getValue();
        Long currRelId = (Long)pair.getKey();
        ReviewableItemBboxInfo info = allReviewables.get(currRelId);
        if(info != null)
        {
        	// expand bbox
        	info.getBbox().add(currInfo.getBbox());
        }
        else
        {
        	// just add
        	allReviewables.put(currRelId, currInfo);
        }
    }
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
	}

	protected String _getReviewableRelationMembersCountByTypeQuery(final String memberType)
	{
		return "select count(*) as cnt from current_relation_members_" + getMapId() + " where relation_id in (" +
						"select id from current_relations_" + getMapId() + " where exist(tags,'hoot:review:needs'))" +					
					"and member_type='" + memberType + "'";
	}
	
	protected long _getReviewableRelationMembersCount(final String type) throws Exception
	{
		long recordCount = 0;
		
		try(				
				Statement stmt = getConnection().createStatement();
				ResultSet rs = stmt.executeQuery(_getReviewableRelationMembersCountByTypeQuery(type))
				)
		{
			while(rs.next())
      {
				recordCount = rs.getLong("cnt");         
      }
			
			SQLQuery query = new SQLQuery(getConnection(), DbUtils.getConfiguration(getMapId()));

		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}

		
		return recordCount;
	}
	
	private BoundingBox _resultSetToBbox(final ResultSet rs) throws Exception
	{
		BoundingBox bbox = null;
		double minLon=-1, minLat=-1, maxLon=-1, maxLat=-1;
		try
		{
			minLon = rs.getDouble("minlon");
			minLat = rs.getDouble("minlat");
			maxLon = rs.getDouble("maxlon");
			maxLat = rs.getDouble("maxlat");
			
			bbox = new BoundingBox( minLon,  minLat, maxLon,  maxLat);
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());;
			throw ex;
		}
		return bbox;
	}
	
	protected Statement _createStatement() throws Exception
	{
		return getConnection().createStatement();
	}
	protected ResultSet _execReviewableRelatioWithWayMembersCentroidInBboxQuery(final Statement stmt) throws Exception
	{
		return stmt.executeQuery(_getReviewableRelatioWithWayMembersCentroidInBboxQuery());
	}
	
	protected Map<Long, ReviewableItemBboxInfo> _getReviewableRelatioWithWayMembersCentroidInBbox() throws Exception
	{
		Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();
		
		try(
				Statement stmt = _createStatement();
				ResultSet rs = _execReviewableRelatioWithWayMembersCentroidInBboxQuery(stmt)
				)
		{
			while(rs.next())
      {
				long relId = rs.getLong("relation_id");
				String needReview = rs.getString("needreview");
				BoundingBox bbox = _resultSetToBbox(rs);
				ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
				relationBboxMap.put(relId, info);
      }
			
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return relationBboxMap;
	}
	
	
	protected String _getReviewableRelatioWithWayMembersCentroidInBboxQuery() throws Exception
	{
		String sql = "";
		try
		{
			if(_bbox == null)
			{
				throw new Exception("Invalid Bounding box.");
			}
			sql = 
			"SELECT relation_id, needreview, minlat, maxlat, minlon, maxlon FROM " + 
			"(" + 
			"	SELECT reviewRelJoinRelMemberJoinCurrentWayNodesSubQ.relation_id, reviewRelJoinRelMemberJoinCurrentWayNodesSubQ.needreview," +
			"					max(currentNodeSubQ.latitude) as maxlat, min(currentNodeSubQ.latitude) as minlat," + 
			"					max(currentNodeSubQ.longitude) as maxlon, min(currentNodeSubQ.longitude) as minlon," +
			"					(((max(currentNodeSubQ.latitude) - min(currentNodeSubQ.latitude))/2)+min(currentNodeSubQ.latitude)) AS centlat, " +
			"					(((max(currentNodeSubQ.longitude) - min(currentNodeSubQ.longitude))/2)+min(currentNodeSubQ.longitude)) AS centlon " + 
			"	FROM" + 
			"	(" + 
			"		SELECT id, latitude  , longitude FROM current_nodes_"  + getMapId() +
			"	) AS currentNodeSubQ" + 
			"	JOIN" + 
			"	(" + 
			"		SELECT currentWayNodesSubQ.node_id, reviewRelJoinRelMemberSubQ.relation_id, reviewRelJoinRelMemberSubQ.needreview FROM" + 
			"		(" + 
			"			SELECT node_id, way_id FROM current_way_nodes_"  + getMapId() +
			"		) AS currentWayNodesSubQ" + 
			"		JOIN" + 
			"		(" + 
			"			SELECT currentRelMembersSubQ.member_id, currentRelMembersSubQ.relation_id, reviewableCurrentRelSubQ.needreview " + 
			"			FROM " + 
			"			(" + 
			"				SELECT member_id, relation_id, member_type FROM current_relation_members_" + getMapId() +
			"			) AS currentRelMembersSubQ" + 
			"			JOIN" + 
			"			(" + 
			"				SELECT id, tags->'hoot:review:needs' AS needreview  FROM current_relations_" + getMapId() +" WHERE exist(tags,'hoot:review:needs')" + 
			"			) AS reviewableCurrentRelSubQ " + 
			"			ON (currentRelMembersSubQ.relation_id=reviewableCurrentRelSubQ.id)  " + 
			"			AND currentRelMembersSubQ.member_type='way'" + 
			"		) AS reviewRelJoinRelMemberSubQ " + 
			"		ON (currentWayNodesSubQ.way_id=reviewRelJoinRelMemberSubQ.member_id)" + 
			"	) AS reviewRelJoinRelMemberJoinCurrentWayNodesSubQ " + 
			"	ON (currentNodeSubQ.id=reviewRelJoinRelMemberJoinCurrentWayNodesSubQ.node_id)" + 
			"	GROUP BY reviewRelJoinRelMemberJoinCurrentWayNodesSubQ.relation_id, reviewRelJoinRelMemberJoinCurrentWayNodesSubQ.needreview" + 
			") AS reviewRelWayMembersCentroidSubQ " +
			"WHERE" +
			" centlat>=" +
			_bbox.getMinLat() +
			" AND centlat<=" +
			_bbox.getMaxLat() +
			" AND centlon>=" +
			_bbox.getMinLon() +
			" AND centlon<=" +
			_bbox.getMaxLon();
		} 
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return sql;
	}
	
	protected ResultSet _execReviewableRelatioWithNodeMembersCentroidInBboxQuery(final Statement stmt) throws Exception
	{
		return stmt.executeQuery(_getReviewableRelatioWithNodeMembersCentroidInBboxQuery());
	}
	
	protected Map<Long, ReviewableItemBboxInfo> _getReviewableRelatioWithNodeMembersCentroidInBbox() throws Exception
	{
		Map<Long, ReviewableItemBboxInfo> relationBboxMap = new HashMap<>();
		
		try(
				Statement stmt = _createStatement();
				ResultSet rs = _execReviewableRelatioWithNodeMembersCentroidInBboxQuery(stmt)
				)
		{
			while(rs.next())
      {
				long relId = rs.getLong("relation_id");
				String needReview = rs.getString("needreview");
				BoundingBox bbox = _resultSetToBbox(rs);
				ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbox, getMapId(), relId, needReview);
				relationBboxMap.put(relId, info);
      }
			
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return relationBboxMap;
	}

	
	protected String _getReviewableRelatioWithNodeMembersCentroidInBboxQuery() throws Exception
	{
		String sql = "";
		try
		{
			if(_bbox == null)
			{
				throw new Exception("Invalid Bounding box.");
			}
			sql = 
					
			"SELECT relation_id, needreview, minlat, maxlat, minlon, maxlon FROM" + 
			"(" + 
			"	SELECT reviewRelJoinRelMemberSubQ.relation_id, reviewRelJoinRelMemberSubQ.needreview," +
			"	max(currentNodeSubQ.latitude) as maxlat, min(currentNodeSubQ.latitude) as minlat," + 
			"	max(currentNodeSubQ.longitude) as maxlon, min(currentNodeSubQ.longitude) as minlon," +
			" (((max(currentNodeSubQ.latitude) - min(currentNodeSubQ.latitude))/2)+min(currentNodeSubQ.latitude)) AS centlat," +
			" (((max(currentNodeSubQ.longitude) - min(currentNodeSubQ.longitude))/2)+min(currentNodeSubQ.longitude)) AS centlon " + 
			"	FROM" + 
			"	(" + 
			"		SELECT id, latitude  , longitude FROM current_nodes_" + getMapId() +
			"	) AS currentNodeSubQ" + 
			"	JOIN" + 
			"	(" + 
			"		SELECT currentRelMembersSubQ.member_id, currentRelMembersSubQ.relation_id, reviewableCurrentRelSubQ.needreview " + 
			"		FROM " + 
			"		(" + 
			"			SELECT member_id, relation_id, member_type FROM current_relation_members_" + getMapId() +
			"		) AS currentRelMembersSubQ" + 
			"		JOIN" + 
			"		(" + 
			"			SELECT id, tags->'hoot:review:needs' AS needreview  FROM current_relations_10 WHERE exist(tags,'hoot:review:needs')" + 
			"		) AS reviewableCurrentRelSubQ " + 
			"		ON (currentRelMembersSubQ.relation_id=reviewableCurrentRelSubQ.id)  " + 
			"		AND currentRelMembersSubQ.member_type='node'" + 
			"	) AS reviewRelJoinRelMemberSubQ  " + 
			"	ON (currentNodeSubQ.id=reviewRelJoinRelMemberSubQ.member_id)" + 
			"	GROUP BY reviewRelJoinRelMemberSubQ.relation_id, reviewRelJoinRelMemberSubQ.needreview" + 
			"	) AS reviewRelNodeMembersCentroidSubQ"		+				
			"	WHERE" +
			" centlat>=" +
			_bbox.getMinLat() +
			" AND centlat<=" +
			_bbox.getMaxLat() +
			" AND centlon>=" +
			_bbox.getMinLon() +
			" AND centlon<=" +
			_bbox.getMaxLon();
		} 
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return sql;
	}

	protected ResultSet _execReviewableRelationMembersQuery(final Statement stmt) throws Exception
	{
		return stmt.executeQuery(_getReviewableRelationMembersQuery());
	}
	
	protected List<JSONObject> _getReviewableRelationMembers() throws Exception
	{
		List<JSONObject> relations = new ArrayList<>();
		
		try(
				Statement stmt = _createStatement();
				ResultSet rs = _execReviewableRelationMembersQuery(stmt)
				)
		{
			while(rs.next())
			{
				long relId = rs.getLong("relation_id");
				long memberId = rs.getLong("member_id");
				String needReview = rs.getString("needreview");
				JSONObject o = new JSONObject();
				o.put("relationid", relId);
				o.put("memberid", memberId);
				o.put("needreview", needReview);
				relations.add(o);
			}
		}
		
		
		return relations;
	}
	
	
	protected String _getReviewableRelationMembersQuery()
	{
		return
		"SELECT currentRelMembersSubQ.relation_id, currentRelMembersSubQ.member_id,  reviewableCurrentRelSubQ.needreview " + 
		"FROM " + 
		"(" + 
		"	SELECT member_id, relation_id, member_type FROM current_relation_members_" + getMapId() +
		") AS currentRelMembersSubQ" + 
		"JOIN" + 
		"(" + 
		"	SELECT id, tags->'hoot:review:needs' AS needreview FROM current_relations_" + getMapId() + 
		" WHERE exist(tags,'hoot:review:needs')" + 
		") AS reviewableCurrentRelSubQ " + 
		"ON (currentRelMembersSubQ.relation_id=reviewableCurrentRelSubQ.id)  " + 
		"AND currentRelMembersSubQ.member_type='way'" + 
		"GROUP BY currentRelMembersSubQ.member_id, currentRelMembersSubQ.relation_id, reviewableCurrentRelSubQ.needreview ";
		
	}
	

}
