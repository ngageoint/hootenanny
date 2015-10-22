package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Represents a response from marking all reviews resolved
 */
@XmlRootElement
public class ReviewResolverResponse
{
	private long changesetId;
  public long getChangesetId() { return changesetId; }
  public void setChangesetId(long id) { this.changesetId = id; }
  
  public ReviewResolverResponse()
  {
  }
  
  public ReviewResolverResponse(final long changesetId)
  {
  	this.changesetId = changesetId;
  }
}
