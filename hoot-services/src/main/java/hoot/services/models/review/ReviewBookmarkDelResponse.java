package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewBookmarkDelResponse {
	private long deleteCount;
	public long getDeleteCount()
	{
		return deleteCount;
	}
	
	public void setDeleteCount(final long nDel)
	{
		this.deleteCount = nDel;
	}
	
	public ReviewBookmarkDelResponse(final long nDel)
	{
		this.deleteCount = nDel;
	}
	
	public ReviewBookmarkDelResponse()
	{
		
	}
}
