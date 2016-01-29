package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;


@XmlRootElement
public class ReviewBookmarkSaveRequest {
	private long bookmarkId;
	public long getBookmarkId() { return bookmarkId; }
	public void setBookmarkId(final long id) { this.bookmarkId = id; }
	
	private long mapId;
	public long getMapId() { return mapId; }
  public void setMapId(final long id) { this.mapId = id; }
  
  private long relationId;
  public long getRelationId() { return this.relationId;}
  public void setRelationId(final long relId) {this.relationId = relId;}
  
  private JSONObject detail;
  public JSONObject getDetail() { return this.detail;}
  public void setDetail(final JSONObject d) throws Exception {
	  	String jsonStr = d.toJSONString();
	  	JSONParser p = new JSONParser();
	  	try
	  	{
	  		this.detail = (JSONObject)p.parse(jsonStr);
	  	}
	  	catch (Exception ex)
	  	{
	  		throw new Exception("Failed to parse review bookmark detail parameter to JSON: " + ex.getMessage());
	  	}
  	}
  
  private long userId;
  public long getUserid() { return this.userId; }
  public void setUserId(final long uid) { this.userId = uid; }
  
  public ReviewBookmarkSaveRequest(final long mapid, final long relationid, 
  		final JSONObject reviewDetail, final long uid) {
  	this.mapId = mapid;
  	this.relationId = relationid;
  	this.detail = reviewDetail;
  	this.userId = uid;
  }
  
  public ReviewBookmarkSaveRequest() {
  }
}
