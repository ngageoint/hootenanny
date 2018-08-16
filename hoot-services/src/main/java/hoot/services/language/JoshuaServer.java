
package hoot.services.language;

public class JoshuaServer 
{
  private String languageCode;
  private String languagePackPath;
  private int port;

  public JoshuaServer() {}

  public String getLanguageCode() { return languageCode; }
  public void setLanguageCode(String code) { this.languageCode = code; }

  public String getLanguagePackPath() { return languagePackPath; }
  public void setLanguagePackPath(String path) { this.languagePackPath = path; }

  public int getPort() { return port; }
  public void setPort(int port) { this.port = port; }
}
