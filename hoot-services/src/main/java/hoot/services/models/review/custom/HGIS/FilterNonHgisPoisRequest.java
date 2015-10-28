package hoot.services.models.review.custom.HGIS;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class FilterNonHgisPoisRequest {
	private String source;
	public String getSource()
	{
		return this.source;
	}
	
	public void setSource(String src)
	{
		this.source = src;
	}
	private String output;
	
	public String getOutput()
	{
		return this.output;
	}
	
	public void setOutput(String out)
	{
		this.output = out;
	}
}
