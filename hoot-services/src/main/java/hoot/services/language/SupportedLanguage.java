
package hoot.services.language;

/*
*/
public final class SupportedLanguage
{
  private String iso6391Code;
  private String iso6392Code;
  private String language;
  private boolean detectable;

  public SupportedLanguage() {}

  public String getIso6391code() { return iso6391Code; }
  public void setIso6391Code(String code) { this.iso6391Code = code; }

  public String getIso6392code() { return iso6392Code; }
  public void setIso6392Code(String code) { this.iso6392Code = code; }

  public String getLanguage() { return language; }
  public void setLanguage(String language) { this.language = language; }

  public boolean getDetectable() { return detectable; }
  public void setDetectable(String detectable) { this.detectable = detectable; }
}
