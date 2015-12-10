package hoot.services.readers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableItemBbox;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.types.query.ListSubQuery;

public class ReviewableBboxQuery extends ReviewableQueryBase implements IReviewableQuery  {
	private static final Logger log = LoggerFactory.getLogger(ReviewableBboxQuery.class);
	
	private Long _relationId = null;
	
	public ReviewableBboxQuery(final Connection c, final long mapid, final long relationid)
	{
		super(c,mapid);
		_relationId = relationid;
	}
	
	public ReviewQueryMapper execQuery() throws SQLException, Exception
	{
		BoundingBox currBbox = new BoundingBox();
		ReviewableItemBbox ret = new ReviewableItemBbox(currBbox, getMapId(), _relationId);
		List<BoundingBox> membersBboxList = new ArrayList<>();

		try
		{
			// do recursive bbox retrieval since relation may contain other relation
			List<Long> relIds = new ArrayList<>();
			relIds.add(_relationId);
			_getRelationMembersBboxRecursive(membersBboxList, relIds);
			
			for(BoundingBox bbx : membersBboxList)
			{
				currBbox.add(bbx);
			}
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
	
		return ret;
	}
	
	private void _getRelationMembersBboxRecursive(final List<BoundingBox> membersBboxList, final List<Long> relIds) throws Exception
	{
		try
		{
			for(Long relId : relIds)
			{
				BoundingBox nodeMemBbox = _getRelationNodeMembersBbox(relId);
				if(nodeMemBbox != null)
				{
					membersBboxList.add(nodeMemBbox);
				}
				
				BoundingBox wayMemBbox = _getRelationWayMembersBbox(relId);
				if(wayMemBbox != null)
				{
					membersBboxList.add(wayMemBbox);
				}
				List<Long> memRelationIds = _getRelationMembers(relId);
				if(memRelationIds.size() > 0)
				{
					_getRelationMembersBboxRecursive(membersBboxList, memRelationIds);
				}
				
			}
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
	}
	
	private BoundingBox _resultSetToBbox(final Tuple tup) throws Exception
	{
		BoundingBox bbox = null;
		double minLon=-1, minLat=-1, maxLon=-1, maxLat=-1;
		try
		{

			minLat = tup.get(0, Double.class);
			maxLat = tup.get(1, Double.class);
			minLon = tup.get(2, Double.class);
			maxLon = tup.get(3, Double.class);
			
			bbox = new BoundingBox( minLon,  minLat, maxLon,  maxLat);
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());;
			throw ex;
		}
		return bbox;
	}
	
	protected BoundingBox _getRelationWayMembersBbox(final long relationId) throws Exception
	{
		BoundingBox bbox = null;
	
		try
		{
			QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
			SQLQuery sql = _getRelationWayMembersBboxQuery(relationId);
			List<Tuple> tups = sql.list(currentNodes.latitude.min(), currentNodes.latitude.max(),
					currentNodes.longitude.min(),currentNodes.longitude.min());
			for(int i=0; i<tups.size(); i++)
			{
				Tuple tup = tups.get(i);
				bbox = _resultSetToBbox(tup);
			}
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return bbox;
	}
		
	
	
	protected SQLQuery _getRelationWayMembersBboxQuery(final long relationId)
	{
		
		QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
		ListSubQuery<Long> sub = new SQLSubQuery()
		.from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationId)
				.and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))).list(currentRelationMembers.memberId);
		
		QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
		ListSubQuery<Long> wayNodesSub = new SQLSubQuery()
			.from(currentWayNodes)
			.where(currentWayNodes.wayId.in(sub)).list(currentWayNodes.nodeId);
		
		QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
		return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId()))
			.from(currentNodes).where(currentNodes.id.in(wayNodesSub));
		
	/*	"select max(latitude) as maxlat, min(latitude) as minlat, max(longitude) as maxlon, min(longitude) as minlon "
				+ "from current_nodes_"
				+ getMapId()
				+ " where id in"
				+ "(select node_id from current_way_nodes_"
				+ getMapId()
				+ " where way_id in"
				+ "(select member_id from current_relation_members_"
				+ getMapId()
				+ " where relation_id="
				+ relationId
				+ " and member_type='way'))";		*/
	}
	
	
	protected BoundingBox _getRelationNodeMembersBbox(final long relationId) throws Exception
	{
		BoundingBox bbox = null;
		try
		{
			QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
			SQLQuery sql = _getRelationNodeMembersBboxQuery(relationId);
			List<Tuple> tups = sql.list(currentNodes.latitude.min(), currentNodes.latitude.max(),
					currentNodes.longitude.min(),currentNodes.longitude.min());
			for(int i=0; i<tups.size(); i++)
			{
				Tuple tup = tups.get(i);
				bbox = _resultSetToBbox(tup);
			}
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return bbox;
	}	
	
	protected SQLQuery _getRelationNodeMembersBboxQuery(final long relationId)
	{
		
	/*	"select max(latitude) as maxlat, min(latitude) as minlat, max(longitude) as maxlon, min(longitude) as minlon "
				+ "from current_nodes_"
				+ getMapId()
				+ " where id in"
				+ "(select member_id from current_relation_members_"
				+ getMapId()
				+ " where relation_id="
				+ relationId
				+ " and member_type='node')";*/
		
		QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
		ListSubQuery<Long> sub = new SQLSubQuery()
		.from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationId)
				.and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))).list(currentRelationMembers.memberId);
		
		QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
		return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId()))
			.from(currentNodes).where(currentNodes.id.in(sub));
	}
	
	
	
	private List<Long> _getRelationMembers(final long relationId) throws Exception
	{
		List<Long> relMemberIds = new ArrayList<>();
		try
		{
			QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
			relMemberIds = _getRelationMembersQuery(relationId).list(currentRelationMembers.memberId);
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return relMemberIds;
	}
	
	
	protected SQLQuery _getRelationMembersQuery(final long relationId)
	{
		/*"select member_id from current_relation_members_"
				+ getMapId()
				+ " where relation_id="
				+ relationId
				+ " and member_type='relation'";*/
		QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
		return new SQLQuery(this.getConnection(), DbUtils.getConfiguration(this.getMapId()))
		.from(currentRelationMembers).where(currentRelationMembers.relationId.eq(relationId)
				.and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation)));
	}

}

