package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewBookmarksStatResponse {
	private long totalCount;
	
	public void setTotalCount(final long cnt)
	{
		this.totalCount = cnt;
	}
	
	public long getTotalCount()
	{
		return this.totalCount;
	}
	
	public ReviewBookmarksStatResponse(final long cnt)
	{
		this.totalCount = cnt;
	}
	
	public ReviewBookmarksStatResponse(){}
}
