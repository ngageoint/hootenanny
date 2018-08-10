
package hoot.services.controllers.language;

public class LanguageTranslateRequest 
{
  private String translator;
  private String sourceLangCode;
  private String text;

  public LanguageTranslateRequest() {}

  public String getTranslator() { return translator; }
  public void setTranslator(String translator) { this.translator = translator; }

  public String getSourceLangCode() { return sourceLangCode; }
  public void setSourceLangCode(String sourceLangCode) { this.sourceLangCode = sourceLangCode; }

  public String getText() { return text; }
  public void setText(String text) { this.text = text; }
}
