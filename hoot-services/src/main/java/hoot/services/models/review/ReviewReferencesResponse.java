package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Collection of review references
 */
@XmlRootElement
public class ReviewReferencesResponse
{
	private ReviewReferences[] reviewReferenceResponses;
  public ReviewReferences[] getReviewReferenceResponses() { return reviewReferenceResponses; }
  public void setReviewReferenceResponses(ReviewReferences[] responses) 
  { this.reviewReferenceResponses = responses; }
  
  public ReviewReferencesResponse()
  {
  }
  
  public ReviewReferencesResponse(final ReviewReferences[] reviewReferenceResponses)
  {
  	this.reviewReferenceResponses = reviewReferenceResponses;
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	for (ReviewReferences references : reviewReferenceResponses)
  	{
  		str += references.toString();
  	}
  	return str;
  }
}