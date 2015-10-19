package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Response to a review references web request
 */
@XmlRootElement
public class ReviewReferences
{
  private ReviewInfo[] reviewInfoItems;
  public ReviewInfo[] getReviewInfoItems() { return reviewInfoItems; }
  public void setReviewInfoItems(ReviewInfo[] reviewInfoItems) 
  { this.reviewInfoItems = reviewInfoItems; }
  
  private String requestElementUniqueId;
  public String getRequestElementUniqueId() { return requestElementUniqueId; }
  public void setRequestElementUniqueId(String id) { this.requestElementUniqueId = id; }
  
  public ReviewReferences()
  {
  }
  
  @Override
  public String toString()
  {
  	String str = "Request element unique ID: " + requestElementUniqueId + "\nReview info items:\n";
  	for (int i = 0; i < reviewInfoItems.length; i++)
  	{
  		str += reviewInfoItems[i].toString() + "\n";
  	}
  	return str;
  }
}