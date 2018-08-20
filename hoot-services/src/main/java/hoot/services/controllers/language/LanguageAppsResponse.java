
package hoot.services.controllers.language;

import hoot.services.language.LanguageApp;

public class LanguageAppsResponse 
{
  private LanguageApp[] apps;

  public LanguageAppsResponse() {}
  public LanguageAppsResponse(LanguageApp[] apps) 
  {
    this.apps = apps;
  }

  public LanguageApp[] getApps() { return apps; }
  public void setApps(LanguageApp[] apps) { this.apps = apps; }
}
