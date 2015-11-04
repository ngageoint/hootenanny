package hoot.services.models.review;

import java.util.List;

public class ReviewableItemInfoResponse {
	private String _type;
	private List<ReviewableItemInfo> _info;
	
	public void setType(String t)
	{
		_type = t;
	}
	
	public String getType()
	{
		return _type;
	}
	
	public void setInfo(List<ReviewableItemInfo> inf)
	{
		_info = inf;
	}
	
	public List<ReviewableItemInfo> getInfo()
	{
		return  _info;
	}
}
