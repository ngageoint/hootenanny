package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Collection of review references
 */
@XmlRootElement
public class ReviewRefsResponses
{
	private ReviewRefsResponse[] reviewRefsResponses;
  public ReviewRefsResponse[] getReviewRefsResponses() { return reviewRefsResponses; }
  public void setReviewRefsResponses(ReviewRefsResponse[] responses) 
  { this.reviewRefsResponses = responses; }
  
  public ReviewRefsResponses()
  {
  }
  
  public ReviewRefsResponses(final ReviewRefsResponse[] reviewRefsResponses)
  {
  	this.reviewRefsResponses = reviewRefsResponses;
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	for (ReviewRefsResponse refs : reviewRefsResponses)
  	{
  		str += refs.toString();
  	}
  	return str;
  }
}