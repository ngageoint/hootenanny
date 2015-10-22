package hoot.services.models.review;

import hoot.services.models.osm.ElementInfo;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Response to a review references web request
 */
@XmlRootElement
public class ReviewRefsResponse
{
  private ReviewRef[] reviewRefs;
  public ReviewRef[] getReviewRefs() { return reviewRefs; }
  public void setReviewRefs(ReviewRef[] refs) 
  { this.reviewRefs = refs; }
  
  private ElementInfo queryElementInfo;
  public ElementInfo getQueryElementInfo() { return queryElementInfo; }
  public void setQueryElementInfo(ElementInfo info) { this.queryElementInfo = info; }
  
  public ReviewRefsResponse()
  {
  }
  
  public ReviewRefsResponse(final ElementInfo requestingElementInfo, 
  	final ReviewRef[] reviewReferences)
  {
  	this.queryElementInfo = requestingElementInfo;
  	this.reviewRefs = reviewReferences;
  }
  
  @Override
  public String toString()
  {
  	String str = "Request element " + queryElementInfo.toString() + ", Review references:\n";
  	for (int i = 0; i < reviewRefs.length; i++)
  	{
  		str += reviewRefs[i].toString() + "\n";
  	}
  	return str;
  }
}