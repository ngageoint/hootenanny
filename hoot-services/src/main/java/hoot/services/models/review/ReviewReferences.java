package hoot.services.models.review;

import hoot.services.models.osm.ElementInfo;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Response to a review references web request
 */
@XmlRootElement
public class ReviewReferences
{
  private ElementInfo[] reviewReferences;
  public ElementInfo[] getReviewReferences() { return reviewReferences; }
  public void setReviewReferences(ElementInfo[] refs) 
  { this.reviewReferences = refs; }
  
  private ElementInfo queryElementInfo;
  public ElementInfo getQueryElementInfo() { return queryElementInfo; }
  public void setQueryElementInfo(ElementInfo info) { this.queryElementInfo = info; }
  
  public ReviewReferences()
  {
  }
  
  public ReviewReferences(final ElementInfo requestingElementInfo, 
  	final ElementInfo[] reviewReferences)
  {
  	this.queryElementInfo = requestingElementInfo;
  	this.reviewReferences = reviewReferences;
  }
  
  @Override
  public String toString()
  {
  	String str = "Request element " + queryElementInfo.toString() + ", Review references:\n";
  	for (int i = 0; i < reviewReferences.length; i++)
  	{
  		str += reviewReferences[i].toString() + "\n";
  	}
  	return str;
  }
}