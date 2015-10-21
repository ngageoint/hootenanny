package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Collection of review references
 */
@XmlRootElement
public class ReviewRefsResponses
{
	private ReviewRefsResponse[] reviewRefsResponses;
  public ReviewRefsResponse[] getReviewRefResponses() { return reviewRefsResponses; }
  public void setReviewRefResponses(ReviewRefsResponse[] responses) 
  { this.reviewRefsResponses = responses; }
  
  public ReviewRefsResponses()
  {
  }
  
  public ReviewRefsResponses(final ReviewRefsResponse[] reviewRefResponses)
  {
  	this.reviewRefsResponses = reviewRefResponses;
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