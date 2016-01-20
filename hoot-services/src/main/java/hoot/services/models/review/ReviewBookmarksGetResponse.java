package hoot.services.models.review;

import java.util.List;

import javax.xml.bind.annotation.XmlRootElement;

import hoot.services.db2.ReviewBookmarks;

@XmlRootElement
public class ReviewBookmarksGetResponse {
	private List<ReviewBookmarks> reviewBookmarks;
	public List<ReviewBookmarks> getReviewBookmarks()
	{
		return this.reviewBookmarks;
	}
	
	public void setReviewBookmarks(final List<ReviewBookmarks> rTags)
	{
		this.reviewBookmarks = rTags;
	}
	
	public ReviewBookmarksGetResponse(final List<ReviewBookmarks> rTags)
	{
		this.reviewBookmarks = rTags;
	}
	
	public ReviewBookmarksGetResponse()
	{
		
	}
}
