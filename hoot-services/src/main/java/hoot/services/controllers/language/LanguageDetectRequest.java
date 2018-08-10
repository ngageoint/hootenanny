
package hoot.services.controllers.language;

public class LanguageDetectRequest 
{
  private String[] apps;
  private String text;

  public LanguageDetectRequest() {}

  public String[] getApps() { return apps; }
  public void setApps(String[] apps) { this.apps = apps; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }
}
