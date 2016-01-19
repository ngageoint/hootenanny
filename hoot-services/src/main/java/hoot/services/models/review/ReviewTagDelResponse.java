package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewTagDelResponse {
	private long deleteCount;
	public long getDeleteCount()
	{
		return deleteCount;
	}
	
	public void setDeleteCount(final long nDel)
	{
		this.deleteCount = nDel;
	}
	
	public ReviewTagDelResponse(final long nDel)
	{
		this.deleteCount = nDel;
	}
	
	public ReviewTagDelResponse()
	{
		
	}
}
