package hoot.services.models.osm;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Element information
 */
@XmlRootElement
public class ElementInfo
{
	private long elementId = -1;
  public long getElementId() { return elementId; }
  public void setElementId(long id) { this.elementId = id; }
  
  //can be map name or id
  private String mapId;
  public String getMapId() { return mapId; }
  public void setMapId(String id) { this.mapId = id; }
  
  private String elementType;
  public String getElementType() { return elementType; }
  public void setElementType(String type) { this.elementType = type; }
  
  public ElementInfo()
  {
  }
  
  public ElementInfo(final String mapId, final long elementId, final String elementType)
  {
  	this.mapId = mapId;
  	this.elementId = elementId;
  	this.elementType = elementType;
  }
  
  @Override
  public String toString()
  {
  	return "map id: " + mapId + ", element id: " + elementId + ", element type: " + elementType;
  }
}
