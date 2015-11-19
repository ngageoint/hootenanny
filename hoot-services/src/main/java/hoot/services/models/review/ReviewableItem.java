package hoot.services.models.review;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewableItem extends ReviewQueryMapper 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableItem.class);
	private long _sortOrder;
	private long _mapId;
	private long _relationId;
	private long _resultCount = 0;
	
	
	public ReviewableItem(final long sortOrder, final long mapid, final long relationid)
	{
		_sortOrder = sortOrder;
		_mapId = mapid;
		_relationId = relationid;
	}
	
	public long getRelationId()
	{
		return _relationId;
	}
	public void setRelationId(final long relid)
	{
		_relationId = relid;
	}
	
	public long getSortOrder()
	{
		return _sortOrder;
	}
	public void setSortOrder(final long sortOrder)
	{
		_sortOrder = sortOrder;
	}
	
	
	public long getMapId()
	{
		return _mapId;
	}
	
	public void setMapId(final long mapId)
	{
		_mapId = mapId;
	}
	
	public long getResultCount()
	{
		return _resultCount;
	}
	
	public void setResultCount(final long cnt)
	{
		_resultCount = cnt;
	}

	public String toString()
	{
		JSONObject o = new JSONObject();
		o.put("sortorder", _sortOrder);
		o.put("mapid", _mapId);
		o.put("relationid", _relationId);
		
		return o.toJSONString();
	}
	
}
