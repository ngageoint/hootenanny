package hoot.services.models.review;

import java.util.List;

import javax.xml.bind.annotation.XmlRootElement;

import hoot.services.db2.ReviewTags;

@XmlRootElement
public class ReviewTagsGetResponse {
	private List<ReviewTags> reviewTags;
	public List<ReviewTags> getReviewTags()
	{
		return this.reviewTags;
	}
	
	public void setReviewTags(final List<ReviewTags> rTags)
	{
		this.reviewTags = rTags;
	}
	
	public ReviewTagsGetResponse(final List<ReviewTags> rTags)
	{
		this.reviewTags = rTags;
	}
	
	public ReviewTagsGetResponse()
	{
		
	}
}
