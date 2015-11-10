package hoot.services.models.review;

import org.json.simple.JSONObject;

public class ReviewableItemInfo {

	private String _type = "Feature";
	private JSONObject _geometry;
	private JSONObject _properties;

	
	public JSONObject getGeometry()
	{
		return _geometry;
	}
	
	public void setGeometry(JSONObject geom)
	{
		_geometry = geom;
	}
	
	public  JSONObject getProperties()
	{
		return _properties;
	}
	
	public void setProperties(JSONObject prop)
	{
		_properties = prop;
	}
	
	public String getType()
	{
		return _type;
	}
	

	
	public void setType(String t)
	{
		_type = t;
	}
	
}
