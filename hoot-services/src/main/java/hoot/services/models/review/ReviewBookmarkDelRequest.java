package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewBookmarkDelRequest {
	private long mapId;
	public long getMapId() { return mapId; }
  public void setMapId(final long id) { this.mapId = id; }
  
  private long relationId;
  public long getRelationId() { return this.relationId;}
  public void setRelationId(final long relId) {this.relationId = relId;}
  
  public ReviewBookmarkDelRequest(final long mpId, final long relId)
  {
  	this.mapId = mpId;
  	this.relationId = relId;
  }
  
  public ReviewBookmarkDelRequest()
  {
  }
}
