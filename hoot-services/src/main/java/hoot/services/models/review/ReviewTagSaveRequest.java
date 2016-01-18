package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

import org.json.simple.JSONObject;


@XmlRootElement
public class ReviewTagSaveRequest {
	private long mapId;
	public long getMapId() { return mapId; }
  public void setMapId(final long id) { this.mapId = id; }
  
  private long relationId;
  public long getRelationId() { return this.relationId;}
  public void setRelationId(final long relId) {this.relationId = relId;}
  
  private JSONObject detail;
  public JSONObject getDetail() { return this.detail;}
  public void setDetail(final JSONObject d) {this.detail = d;}
  
  private long userId;
  public long getUserid() { return this.userId; }
  public void setUserId(final long uid) { this.userId = uid; }
  
  public ReviewTagSaveRequest(final long mapid, final long relationid, 
  		final JSONObject reviewDetail, final long uid) {
  	this.mapId = mapid;
  	this.relationId = relationid;
  	this.detail = reviewDetail;
  	this.userId = uid;
  }
  
  public ReviewTagSaveRequest() {
  }
}
