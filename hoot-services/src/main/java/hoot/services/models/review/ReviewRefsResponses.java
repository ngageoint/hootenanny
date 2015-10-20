package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Collection of review references
 */
@XmlRootElement
public class ReviewRefsResponses
{
	private ReviewRefsResponse[] reviewRefResponses;
  public ReviewRefsResponse[] getReviewRefResponses() { return reviewRefResponses; }
  public void setReviewRefResponses(ReviewRefsResponse[] responses) 
  { this.reviewRefResponses = responses; }
  
  public ReviewRefsResponses()
  {
  }
  
  public ReviewRefsResponses(final ReviewRefsResponse[] reviewRefResponses)
  {
  	this.reviewRefResponses = reviewRefResponses;
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	for (ReviewRefsResponse refs : reviewRefResponses)
  	{
  		str += refs.toString();
  	}
  	return str;
  }
}