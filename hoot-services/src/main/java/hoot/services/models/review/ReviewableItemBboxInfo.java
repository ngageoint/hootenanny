package hoot.services.models.review;

import org.json.simple.JSONObject;

import hoot.services.geo.BoundingBox;

public class ReviewableItemBboxInfo extends ReviewableItemBbox {

	private String _needReview;
	public ReviewableItemBboxInfo(BoundingBox bbox, long mapid, long relationid, final String needReview) {
	  super(bbox, mapid, relationid);
	  _needReview = needReview;
  }
	
	public String getNeedReview()
	{
		return _needReview;
	}
	
	public void setNeedReview(final String needreview)
	{
		_needReview = needreview;
	}
	@Override
	public String toString() {
		JSONObject o = new JSONObject();
		o.put("bbox", this.getBbox());
		o.put("mapid", this.getMapId());
		o.put("relationid", this.getRelationId());
		o.put("needreview", _needReview);
		
		return o.toJSONString();
	}

}
