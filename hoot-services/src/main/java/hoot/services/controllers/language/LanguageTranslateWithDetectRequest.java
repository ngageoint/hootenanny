
package hoot.services.controllers.language;

public class LanguageTranslateWithDetectRequest 
{
  private String[] detectors;
  private String translator;
  private String[] sourceLangCodes;
  private String text;

  public LanguageTranslateWithDetectRequest() {}

  public String[] getDetectors() { return detectors; }
  public void setDetectors(String[] detectors) { this.detectors = detectors; }

  public String getTranslator() { return translator; }
  public void setTranslator(String translator) { this.translator = translator; }

  public String[] getSourceLangCodes() { return sourceLangCodes; }
  public void setSourceLangCodes(String[] sourceLangCodes) { this.sourceLangCodes = sourceLangCodes; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }
}
