package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewBookmarkDelRequest {
	private long bookmarkId;
	public long getBookmarkId() { return bookmarkId; }
  public void setBookmarkId(final long id) { this.bookmarkId = id; }
  
  
  
  public ReviewBookmarkDelRequest(final long bmkId)
  {
  	this.bookmarkId = bmkId;
  }
  
  public ReviewBookmarkDelRequest()
  {
  }
}
