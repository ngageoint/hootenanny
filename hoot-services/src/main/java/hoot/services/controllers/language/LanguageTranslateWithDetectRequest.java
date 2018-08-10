
package hoot.services.controllers.language;

public class LanguageTranslateWithDetectRequest 
{
  private String sourceLangCode;
  private String text;

  public LanguageDetectRequest() {}

  public String getSourceLangCode() { return sourceLangCode; }
  public void setSourceLangCode(String[] sourceLangCode) { this.sourceLangCode = sourceLangCode; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }
}
