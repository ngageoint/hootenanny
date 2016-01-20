package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewBookmarksSaveResponse {
	private long savedCount;
	
	public void setSavedCount(final long nSaved) 
	{
		this.savedCount = nSaved;
	}
	
	public long getSavedCount()
	{
		return this.savedCount;
	}
	
	public ReviewBookmarksSaveResponse (final long nSaved)
	{
		this.savedCount = nSaved;
	}
	
	public ReviewBookmarksSaveResponse()
	{
		
	}
}
