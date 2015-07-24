package hoot.services.readers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewItems;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.support.Expressions;
import com.mysema.query.types.path.SimplePath;
import com.mysema.query.types.query.ListSubQuery;

public class ReviewableItemsReader {
	private static final Logger log = LoggerFactory.getLogger(ReviewableItemsReader.class);
	
	protected static final QReviewItems reviewItemsTbl = QReviewItems.reviewItems;
	protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
	protected static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
	protected static final QCurrentWays currentWays = QCurrentWays.currentWays;
	protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
	protected static final QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
	protected static final QReviewItems reviewItems = QReviewItems.reviewItems;
	
	private static final SimplePath<Integer> MAX_LON = Expressions.path(Integer.class, "max_lon");
	private static final SimplePath<Integer> MAX_LAT = Expressions.path(Integer.class, "max_lat");
	
	private static final SimplePath<Integer> MIN_LON = Expressions.path(Integer.class, "min_lon");
	private static final SimplePath<Integer> MIN_LAT = Expressions.path(Integer.class, "min_lat");


	private Map<Long, ReviewableItemsBBox> bboxMap = null;
	private long mapId;
  public long getMapId() { return mapId; }

  private Connection conn;
  
  public ReviewableItemsReader(final Connection conn, final String mapIdStr) throws Exception
  {
    this.conn = conn;
    
    QMaps maps = QMaps.maps;
    final long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapIdStr, conn,
    		maps, maps.id, maps.displayName);
    
    this.mapId = mapIdNum;
    bboxMap = new HashMap<Long, ReviewableItemsBBox>();
  }
  
    
  public List<ReviewableItem> getReviewItems(final int numItems,
      final boolean highestReviewScoreFirst, final double reviewScoreThresholdMinimum) throws Exception
  {
  	List<ReviewableItem> ret = new ArrayList<ReviewableItem>();
  	
  	//1. Get current_nodes of node reviewable type
  	_getNodeBoundingBox();
  	//2. Get current_ways of way reviewble type
  	//	a. Get All boundings of reviewable + against
  	_getWayBoundingBox();
  	//3. Get current_relations of relation reviewable type. Recurse using method 1 + 2
  	_getRelationsBoundingBox();
  	
  	//4. when done pacakge into RevieableItem and return
  	
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
		final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
		
		final SimplePath<Double> review_score = Expressions.path(Double.class, "review_score");
		
		final ListSubQuery<Tuple> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) this.mapId)
				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed))
				.and(reviewItems.reviewScore.goe(reviewScoreThresholdMinimum))).orderBy(reviewItems.reviewScore.desc())
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id), reviewItems.reviewScore.as(review_score));
		
		
		
		List<Tuple> res = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.list(currentRelationMembers.relationId, currentRelationMembers.memberId,currentRelationMembers.memberType,currentRelationMembers.sequenceId, review_score);

		Map<Long, ReviewableItem> reviewables = new HashMap<Long, ReviewableItem>();
		Map<Long,List<ReviewAgainstItem>> reviewAgainstMap = new HashMap<Long,List<ReviewAgainstItem>>();
		for(int i=0; i<res.size(); i++)
		{
			Tuple tup = res.get(i);
			
			Long relId = tup.get(currentRelationMembers.relationId);
			Long memId = tup.get(currentRelationMembers.memberId);
			String memType = tup.get(currentRelationMembers.memberType).toString();
			Double revScore = tup.get(review_score);
			Integer seq = tup.get(currentRelationMembers.sequenceId);
			
			
			
			
			
			//ReviewAgainstItem itemToReviewAgainst = new ReviewAgainstItem();
			//rt.setItemToReviewAgainst(itemToReviewAgainst);
			
			// we will assume first is primary for now till we get clarification on review::source tag
			if(seq == 0)
			{
			
				// primary
				ReviewableItem rt = new ReviewableItem();
				rt.setId(memId);
				rt.setReviewScore(revScore);
				rt.setType(memType);
				rt.setUuid(""); // remove me
				
				ReviewableItemsBBox b = bboxMap.get(relId);
				if(b != null)
				{/*
					min lon
					min lat
					max lon
					max lat*/
					String displayBounds = "" + DbUtils.fromDbCoordValue(b.getMinLon()) + "," + DbUtils.fromDbCoordValue(b.getMinLat()) 
							+ "," + DbUtils.fromDbCoordValue(b.getMaxLon()) + "," + DbUtils.fromDbCoordValue(b.getMaxLat());
					rt.setDisplayBounds(displayBounds);
					
				}
				
				reviewables.put(relId, rt);
			}
			else
			{
				// review against
				ReviewAgainstItem itemToReviewAgainst = new ReviewAgainstItem();
				itemToReviewAgainst.setId(memId);
				itemToReviewAgainst.setType(memType);
				itemToReviewAgainst.setUuid(""); // remove me
				
				//
				List<ReviewAgainstItem> items = reviewAgainstMap.get(relId);
				if(items == null)
				{
					items = new ArrayList<ReviewAgainstItem>();
				}
				items.add(itemToReviewAgainst);
				reviewAgainstMap.put(relId, items);
			}
			
		
		}
		
		
		
		// construct the list
		
		Iterator it = reviewables.entrySet().iterator();
    while (it.hasNext()) {
        Map.Entry pair = (Map.Entry)it.next();
        Long k = (Long) pair.getKey();
        ReviewableItem v = (ReviewableItem) pair.getValue();
        
        if(reviewAgainstMap.containsKey(k))
        {
        	List<ReviewAgainstItem> againstItems = reviewAgainstMap.get(k);
        	for(int j=0; j<againstItems.size(); j++)
        	{
        		ReviewAgainstItem itm = againstItems.get(j);
        		
        		ReviewableItem rt = new ReviewableItem();
    				rt.setId(v.getId());
    				rt.setReviewScore(v.getReviewScore());
    				rt.setType(v.getType());
    				rt.setUuid(""); // remove me
    				rt.setItemToReviewAgainst(itm);
    				rt.setDisplayBounds(v.getDisplayBounds());
    				
    				ret.add(rt);
        	}
        }
        // don't return if there is no reviewagainst?
     /*   else
        {
        	ReviewableItem rt = new ReviewableItem();
  				rt.setId(v.getId());
  				rt.setReviewScore(v.getReviewScore());
  				rt.setType(v.getType());
  				rt.setUuid(""); // remove me
  				rt.setDisplayBounds(v.getDisplayBounds());
  				ret.add(rt);
        }*/
    }
		
  	
  	return ret;
  }
  
  protected List<Tuple> _getWayBoundingBox() throws Exception
  {
  	final SimplePath<String> cwn = Expressions.path(String.class, "cwn");
  	Expressions.path(Long.class, "cwn.wayid");
  	final SimplePath<Long> cwn_nodeid = Expressions.path(Long.class, "cwn.nodeid");
  	
  	final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	
  	final SimplePath<String> reviewRelJoin = Expressions.path(String.class, "review_rel_id_join_rel_members");
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids");
  	
  	Expressions.path(String.class, "cr_cn");
  	Expressions.path(String.class, "rel_rel");
  	
  	ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) this.mapId))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));

  	
  	ListSubQuery<Tuple> relationWayMembers = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))
  			.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);
  	

  	final ListSubQuery<Tuple> currentWayNodeIds =  new SQLSubQuery().from(currentWayNodes).
  			join(relationWayMembers,reviewRelJoin).on(currentWayNodes.wayId.eq(rel_mem_ids))
  			.list(review_rel_id, currentWayNodes.nodeId.as(cwn_nodeid));
  	
  	
  	SQLQuery wayQuery = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentNodes).
  			join(currentWayNodeIds, cwn).on(currentNodes.id.eq(cwn_nodeid))
  			.groupBy(review_rel_id);
  	
  	List<Tuple> result = wayQuery.list(review_rel_id, currentNodes.latitude.max().as(MAX_LAT), 
  			currentNodes.latitude.min().as(MIN_LAT),
  			currentNodes.longitude.max().as(MAX_LON),
  			currentNodes.longitude.min().as(MIN_LON));
  	_populateBbox(result);
  	return result;
  }
  
  
  protected List<Tuple> _getNodeBoundingBox() throws Exception
  {
  	Expressions.path(String.class, "cwn");
  	Expressions.path(Long.class, "cwn.wayid");
  	Expressions.path(Long.class, "cwn.nodeid");
  	
  	final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	
  	Expressions.path(String.class, "review_rel_id_join_rel_members");
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids");
  	
  	final SimplePath<String> cr_cn = Expressions.path(String.class, "cr_cn");
  	Expressions.path(String.class, "rel_rel");

  	
  	ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) this.mapId))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));
  	
  	ListSubQuery<Tuple> relationNodeMembers = new SQLSubQuery().from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
  			.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);

  	
  	SQLQuery nodeQuery = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentNodes).
  			join(relationNodeMembers, cr_cn).on(currentNodes.id.eq(rel_mem_ids))
  			.groupBy(review_rel_id);
  	
  	List<Tuple> nodeRes = nodeQuery.list(review_rel_id, currentNodes.latitude.max().as(MAX_LAT), 
  			currentNodes.latitude.min().as(MIN_LAT),
  			currentNodes.longitude.max().as(MAX_LON),
  			currentNodes.longitude.min().as(MIN_LON));
  	_populateBbox(nodeRes);
  	return nodeRes;
  }
  
 
  protected void _getRelationsBoundingBox() throws Exception
  {
  	Expressions.path(String.class, "cwn");
  	Expressions.path(Long.class, "cwn.wayid");
  	Expressions.path(Long.class, "cwn.nodeid");
  	
  	final SimplePath<String> review_rel = Expressions.path(String.class, "review_rel");
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	
  	Expressions.path(String.class, "review_rel_id_join_rel_members");
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids");
  	
  	Expressions.path(String.class, "cr_cn");
  	Expressions.path(String.class, "rel_rel");
  	
  	// select way from relation_members table
  	//	get min max xy and add to store
  	

  	

  	ListSubQuery<Long> reviewableItemRelation = new SQLSubQuery().from(reviewItems).where(reviewItems.mapId.eq((long) this.mapId))
  			.list(reviewItems.reviewableItemRelId.as(review_rel_id));

  
  	
  	
  	SQLQuery relQuery = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentRelationMembers).
  			join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  			.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation));
  	
  	long relCnt = relQuery.count();
  	
  	if(relCnt > 0)
  	{

  		
  		ListSubQuery<Tuple> relationTuples = new SQLSubQuery().from(currentRelationMembers).
  				join(reviewableItemRelation,review_rel).on(currentRelationMembers.relationId.eq(review_rel_id))
  				.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation))
  	  		.list(currentRelationMembers.memberId.as(rel_mem_ids),review_rel_id);
  		
  		int nDepth = 0;
  		_recurseRelations( relationTuples, nDepth);
  	}

  	
  	
  }
  
  protected List<Tuple> _getRelWay(final ListSubQuery<Tuple> relationTuple, int nDepth) throws Exception
  {
  	String relMemIdStr = "";
  	if((nDepth-1) > 0)
  	{
  		relMemIdStr += (nDepth-1);
  	}
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	final SimplePath<String> cwn = Expressions.path(String.class, "cwn" + nDepth);
  	final SimplePath<Long> cwn_nodeid = Expressions.path(Long.class, "cwn.nodeid" + nDepth);
  	final SimplePath<String> wayRelJoin = Expressions.path(String.class, "way_id_join_rel_members" + nDepth);
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids" + relMemIdStr);
  	final SimplePath<String> relTuple = Expressions.path(String.class, "relTuple" + nDepth);
   	
  	ListSubQuery<Tuple> relationWayMembers = new SQLSubQuery().from(currentRelationMembers)
  			.join(relationTuple, relTuple).on(currentRelationMembers.relationId.eq(rel_mem_ids))
  	  	.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))
  	  	.list(review_rel_id, currentRelationMembers.memberId.as(rel_mem_ids));
  	

  	
  	final ListSubQuery<Tuple> currentWayNodeIds =  new SQLSubQuery().from(currentWayNodes).
  			join(relationWayMembers,wayRelJoin).on(currentWayNodes.wayId.eq(rel_mem_ids))
  			.list(review_rel_id,currentWayNodes.nodeId.as(cwn_nodeid));
  	
  	
  	SQLQuery wayQuery = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentNodes).
  			join(currentWayNodeIds, cwn).on(currentNodes.id.eq(cwn_nodeid))
  			.groupBy(review_rel_id);
  	
  	List<Tuple> result = wayQuery.list(review_rel_id, currentNodes.latitude.max().as(MAX_LAT), 
  			currentNodes.latitude.min().as(MIN_LAT),
  			currentNodes.longitude.max().as(MAX_LON),
  			currentNodes.longitude.min().as(MIN_LON));
  	return result;
  }
  
  protected List<Tuple> _getRelNode (final ListSubQuery<Tuple> relationTuple, int nDepth) throws Exception
  {
  	String relMemIdStr = "";
  	if((nDepth-1) > 0)
  	{
  		relMemIdStr += (nDepth-1);
  	}
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id" + relMemIdStr);
  	final SimplePath<Long> rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids" + nDepth);
  	
  	final SimplePath<String> cr_cn = Expressions.path(String.class, "cr_cn" + nDepth);
  	final SimplePath<String> relTuple = Expressions.path(String.class, "relTuple" + nDepth);

  	
  	ListSubQuery<Tuple> relationNodeMembers = new SQLSubQuery().from(currentRelationMembers)
  			.join(relationTuple, relTuple).on(currentRelationMembers.relationId.eq(rel_mem_ids))
  	  	.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
  			.list(currentRelationMembers.memberId.as(rel_mem_ids), review_rel_id);

  	
  	SQLQuery nodeQuery = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentNodes).
  			join(relationNodeMembers, cr_cn).on(currentNodes.id.eq(rel_mem_ids))
  			.groupBy(review_rel_id);
  	
  	List<Tuple> nodeRes = nodeQuery.list(review_rel_id, currentNodes.latitude.max().as(MAX_LAT), 
  			currentNodes.latitude.min().as(MIN_LAT),
  			currentNodes.longitude.max().as(MAX_LON),
  			currentNodes.longitude.min().as(MIN_LON));
  	
  	return nodeRes;
  }
  
  
  protected void _recurseRelations(final ListSubQuery<Tuple> relationTuple, int nDepth) throws Exception
  {
  	String relMemIdStr = "rel_mem_ids";
  	if(nDepth > 0)
  	{
  		relMemIdStr += nDepth;
  	}
  	final SimplePath<Long> last_rel_mem_ids = Expressions.path(Long.class, relMemIdStr);
  	final SimplePath<Long> curr_rel_mem_ids = Expressions.path(Long.class, "rel_mem_ids" + nDepth);
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	int curDepth = nDepth + 1;
  	final SimplePath<String> relTuple = Expressions.path(String.class, "relTuple" + nDepth);
/*
  	new SQLQuery(conn, DbUtils.getConfiguration(4)).from(currentRelationMembers)
  	  	.where(currentRelationMembers.relationId.in(relationIds).and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)));
  	
  	new SQLQuery(conn, DbUtils.getConfiguration(4)).from(currentRelationMembers)
  	.where(currentRelationMembers.relationId.in(relationIds).and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)));*/
  	List<Tuple> nodesBbox = _getRelNode(relationTuple, curDepth);
  	_populateBbox(nodesBbox);
  	List<Tuple> wayBbox = _getRelWay(relationTuple, curDepth);
  	_populateBbox(wayBbox);
  	
  	
  	
  	
  	SQLQuery relQ = new SQLQuery(conn, DbUtils.getConfiguration(this.mapId)).from(currentRelationMembers)
  			.join(relationTuple, relTuple).on(currentRelationMembers.relationId.eq(last_rel_mem_ids))
  	  	.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation));
  	
  	long nRel = relQ.count();
  	if(nRel > 0)
  	{
  		ListSubQuery<Tuple> relationIdsQuery = new SQLSubQuery().from(currentRelationMembers)
    			.join(relationTuple, relTuple).on(currentRelationMembers.relationId.eq(last_rel_mem_ids))
    	  	.where(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation))
    	  	.list(currentRelationMembers.memberId.as(curr_rel_mem_ids), review_rel_id);
  		
  		_recurseRelations(relationIdsQuery, curDepth);
  	}
  }
  
  protected void _populateBbox(List<Tuple> raw) throws Exception
  {
  	final SimplePath<Long> review_rel_id = Expressions.path(Long.class, "review_rel_id");
  	for(int i=0; i<raw.size(); i++)
  	{
  		Tuple tup = raw.get(i);
  		
  		Long relId = tup.get(review_rel_id);
  		Integer maxLat = tup.get(MAX_LAT);
  		Integer minLat = tup.get(MIN_LAT);
  		Integer maxLon = tup.get(MAX_LON);
  		Integer minLon = tup.get(MIN_LON);
  		
  		ReviewableItemsBBox bbox = new ReviewableItemsBBox(minLon,maxLon,minLat,maxLat);
  		if(bboxMap.containsKey(relId))
  		{
  			bbox = bboxMap.get(relId);
  			
  			Integer curMaxLat = bbox.getMaxLat();
  			if(maxLat > curMaxLat)
  			{
  				bbox.setMaxLat(maxLat);
  			}
  			Integer curMinLat = bbox.getMinLat();
  			if(minLat < curMinLat)
  			{
  				bbox.setMinLat(minLat);
  			}
  			Integer curMaxLon = bbox.getMaxLon();
  			if(maxLon > curMaxLon)
  			{
  				bbox.setMaxLon(maxLon);
  			}
  			Integer curMinLon = bbox.getMinLon();
  			if(minLon < curMinLon)
  			{
  				bbox.setMinLon(minLon);
  			}
  		}
  		bboxMap.put(relId, bbox);
  	}
  }
 
}

class ReviewableItemsBBox
{
	private Integer _minLon = 0;
	private Integer _maxLon = 0;
	private Integer _minLat = 0;
	private Integer _maxLat = 0;
	
	public ReviewableItemsBBox()
	{
	
	}
	
	public ReviewableItemsBBox(Integer minLon,Integer maxLon,Integer minLat,Integer maxLat)
	{
		_minLon = minLon;
		_maxLon = maxLon;
		_minLat = minLat;
		_maxLat = maxLat;
	}
	
	/**
	 * @return the _minLon
	 */
  public Integer getMinLon() {
	  return _minLon;
  }
	/**
	 * @param _minLon the _minLon to set
	 */
  public void setMinLon(Integer _minLon) {
	  this._minLon = _minLon;
  }
	/**
	 * @return the _maxLon
	 */
  public Integer getMaxLon() {
	  return _maxLon;
  }
	/**
	 * @param _maxLon the _maxLon to set
	 */
  public void setMaxLon(Integer _maxLon) {
	  this._maxLon = _maxLon;
  }
	/**
	 * @return the _minLat
	 */
  public Integer getMinLat() {
	  return _minLat;
  }
	/**
	 * @param _minLat the _minLat to set
	 */
  public void setMinLat(Integer _minLat) {
	  this._minLat = _minLat;
  }
	/**
	 * @return the _maxLat
	 */
  public Integer getMaxLat() {
	  return _maxLat;
  }
	/**
	 * @param _maxLat the _maxLat to set
	 */
  public void setMaxLat(Integer _maxLat) {
	  this._maxLat = _maxLat;
  }
}
