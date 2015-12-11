package hoot.services.models.review;

import hoot.services.geo.BoundingBox;

import java.util.Iterator;
import java.util.Map;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class AllReviewableItems extends ReviewQueryMapper {

	private long _mapId;
	private boolean _overflow = false;
	private Map<Long, ReviewableItemBboxInfo> _reviewableItems;
	
	public AllReviewableItems(final long mapid, final Map<Long, ReviewableItemBboxInfo> reviewableItems)
	{
		_mapId = mapid;
		_reviewableItems = reviewableItems;
	}
	
	public long getMapId()
	{
		return _mapId;
	}
	
	public void setMapId(final long mapid)
	{
		_mapId = mapid;
	}
	
	public Map<Long, ReviewableItemBboxInfo> getReviewableItems()
	{
		return _reviewableItems;
	}
	
	public void setReviewableItems(Map<Long, ReviewableItemBboxInfo> reviewableItems)
	{
		_reviewableItems = reviewableItems;
	}
	
	public boolean getOverFlow()
	{
		return _overflow;
	}
	
	public void setOverFlow(final boolean isOverFlow) {
		_overflow = isOverFlow;
	}
	@Override
	public String toString() {
		JSONObject o = new JSONObject();
		o.put("mapid", _mapId);
		o.put("reviewableItems", _reviewableItems);
		return o.toJSONString();
	}
	
	public JSONObject toGeoJson()
	{
		JSONArray features = new JSONArray();
		
		if(_reviewableItems != null)
		{
			Iterator it = _reviewableItems.entrySet().iterator();
	    while (it.hasNext()) {
	        Map.Entry pair = (Map.Entry)it.next();
	        Long relId = (Long)pair.getKey();
	        ReviewableItemBboxInfo info = (ReviewableItemBboxInfo)pair.getValue();
	        
	        JSONObject feature = new JSONObject();
	        feature.put("type", "Feature");
	        
	        JSONObject geometry = new JSONObject();
	        geometry.put("type", "Point");
	        BoundingBox bbx = info.getBbox();
	        double[] coords = new double[] {bbx.getCenterX(), bbx.getCenterY()};
	        geometry.put("coordinates", coords);
	        feature.put("geometry", geometry);
	        
	        JSONObject prop = new JSONObject();
	        String sClass = "open";
	        if(info.getNeedReview().equalsIgnoreCase("no"))
	        {
	        	sClass = "resolved";
	        }
	        prop.put("class", sClass);
	        prop.put("relationid", relId);
	        feature.put("properties", prop);
	
	        features.add(feature);
	    }
		}
		
		JSONObject geojson = new JSONObject();
		geojson.put("type", "FeatureCollection");
		geojson.put("features", features);
		
		return geojson;
		
	}

}
