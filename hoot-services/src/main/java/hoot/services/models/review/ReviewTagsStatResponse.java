package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewTagsStatResponse {
	private long totalCount;
	
	public void setTotalCount(final long cnt)
	{
		this.totalCount = cnt;
	}
	
	public long getTotalCount()
	{
		return this.totalCount;
	}
	
	public ReviewTagsStatResponse(final long cnt)
	{
		this.totalCount = cnt;
	}
	
	public ReviewTagsStatResponse(){}
}
