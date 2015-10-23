package hoot.services.models.review;

import hoot.services.models.osm.ElementInfo;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Element information
 */
@XmlRootElement
public class ReviewRef extends ElementInfo
{
	private long reviewRelationId = -1;
  public long getReviewRelationId() { return reviewRelationId; }
  public void setReviewRelationId(long id) { this.reviewRelationId = id; }
  
  public ReviewRef()
  {
  	super();
  }
  
  public ReviewRef(final String mapId, final long elementId, final String elementType, 
  	final long reviewRelationId)
  {
  	super(mapId, elementId, elementType);
  	this.reviewRelationId = reviewRelationId;
  }
  
  @Override
  public String toString()
  {
  	return super.toString() + ", review relation id: " + reviewRelationId;
  }
}
