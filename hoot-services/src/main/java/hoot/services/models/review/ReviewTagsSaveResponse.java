package hoot.services.models.review;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ReviewTagsSaveResponse {
	private long savedCount;
	
	public void setSavedCount(final long nSaved) 
	{
		this.savedCount = nSaved;
	}
	
	public long getTagid()
	{
		return this.savedCount;
	}
	
	public ReviewTagsSaveResponse (final long nSaved)
	{
		this.savedCount = nSaved;
	}
	
	public ReviewTagsSaveResponse()
	{
		
	}
}
