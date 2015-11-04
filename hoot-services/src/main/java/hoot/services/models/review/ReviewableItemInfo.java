package hoot.services.models.review;

public class ReviewableItemInfo {
	private double _lat;
	private double _lon;
	private String _status;
	
	public double getLat()
	{
		return _lat;
	}
	
	public void setLat(double y)
	{
		_lat = y;
	}
	
	public double getLon()
	{
		return _lon;
	}
	
	public void setLon(double x)
	{
		_lon = x;
	}
	
	
	public void setStatus(String stat)
	{
		_status = stat;
	}
	
	public String getStatus()
	{
		return _status;
	}
	
}
