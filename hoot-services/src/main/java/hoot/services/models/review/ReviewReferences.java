package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Response to a review references web request
 * 
 * see comment in ReviewResource::getReviewReferences as to why ReviewAgainstItem is used for
 * for both review against and reviewable items here
 */
@XmlRootElement
public class ReviewReferences
{
  private ReviewAgainstItem[] reviewAgainstItems;
  public ReviewAgainstItem[] getReviewAgainstItems() { return reviewAgainstItems; }
  public void setReviewAgainstItems(ReviewAgainstItem[] items) { this.reviewAgainstItems = items; }
  
  private ReviewAgainstItem[] reviewableItems;
  public ReviewAgainstItem[] getReviewableItems() { return reviewableItems; }
  public void setReviewableItems(ReviewAgainstItem[] items) { this.reviewableItems = items; }
  
  private String uniqueId;
  public String getUniqueId() { return uniqueId; }
  public void setUniqueId(String id) { this.uniqueId = id; }
  
  public ReviewReferences()
  {
  }
  
  @Override
  public String toString()
  {
  	String str = "Unique ID: " + uniqueId + "\nReview against items:\n";
  	for (int i = 0; i < reviewAgainstItems.length; i++)
  	{
  		str += reviewAgainstItems[i].toString() + "\n";
  	}
  	str += "Reviewable items:\n";
  	for (int i = 0; i < reviewableItems.length; i++)
  	{
  		str += reviewableItems[i].toString() + "\n";
  	}
  	return str;
  }
}
