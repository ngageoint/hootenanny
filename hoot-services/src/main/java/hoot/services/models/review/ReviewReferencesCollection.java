package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Collection of review references
 */
@XmlRootElement
public class ReviewReferencesCollection
{
	private ReviewReferences[] reviewReferences;
  public ReviewReferences[] getReviewReferences() { return reviewReferences; }
  public void setReviewReferences(ReviewReferences[] references) { this.reviewReferences = references; }
  
  public ReviewReferencesCollection()
  {
  }
  
  @Override
  public String toString()
  {
  	String str = "";
  	for (ReviewReferences references : reviewReferences)
  	{
  		str += references.toString();
  	}
  	return str;
  }
}
