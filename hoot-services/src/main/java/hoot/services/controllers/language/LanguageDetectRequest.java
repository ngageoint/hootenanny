
package hoot.services.controllers.language;

public class LanguageDetectRequest 
{
  private String[] detectors;
  private String text;

  public LanguageDetectRequest() {}

  public String[] getDetectors() { return detectors; }
  public void setDetectors(String[] detectors) { this.detectors = detectors; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }
}
