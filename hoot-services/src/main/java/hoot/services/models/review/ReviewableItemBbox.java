package hoot.services.models.review;

import org.json.simple.JSONObject;

import hoot.services.geo.BoundingBox;

public class ReviewableItemBbox extends ReviewQueryMapper {

	private long _mapId;
	private long _relationId;
	private BoundingBox _bbox;
	
	public ReviewableItemBbox(final BoundingBox bbox, final long mapid, final long relationid)
	{
		_bbox = bbox;
		_mapId = mapid;
		_relationId = relationid;
	}
	
	@Override
	public String toString() {
		JSONObject o = new JSONObject();
		o.put("bbox", _bbox.toServicesString());
		o.put("mapid", _mapId);
		o.put("relationid", _relationId);
		
		return o.toJSONString();
	}

	public long getRelationId()
	{
		return _relationId;
	}
	public void setRelationId(final long relid)
	{
		_relationId = relid;
	}
	
	public BoundingBox getBbox()
	{
		return _bbox;
	}
	
	public void setBbox(final BoundingBox bbox)
	{
		_bbox = bbox;
	}
	
	public long getMapId()
	{
		return _mapId;
	}
	
	public void setMapId(final long mapid)
	{
		_mapId = mapid;
	}
}
