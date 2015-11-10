package hoot.services.models.review;

import java.util.List;

public class ReviewableItemInfoResponse {
	private String _type;
	private List<ReviewableItemInfo> _features;
	
	public void setType(String t)
	{
		_type = t;
	}
	
	public String getType()
	{
		return _type;
	}
	
	public void setFeatures(List<ReviewableItemInfo> f)
	{
		_features = f;
	}
	
	public List<ReviewableItemInfo> getFeatures()
	{
		return  _features;
	}
}
