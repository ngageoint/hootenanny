
package hoot.services.language;

/*
*/
public final class SupportedLanguage
{
  private String iso6391Code;
  private String iso6392Code;
  private String name;
  private boolean available = false;

  public SupportedLanguage() {}

  public String getIso6391code() { return iso6391Code; }
  public void setIso6391Code(String code) { this.iso6391Code = code; }

  public String getIso6392code() { return iso6392Code; }
  public void setIso6392Code(String code) { this.iso6392Code = code; }

  public String getName() { return name; }
  public void setName(String name) { this.name = name; }

  public boolean getAvailable() { return available; }
  public void setAvailable(boolean available) { this.available = available; }
}
