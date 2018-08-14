
package hoot.services.controllers.language;

public class LanguageTranslateRequest 
{
  private String[] detectors;
  private String translator;
  private String[] sourceLangCodes;
  private String text;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages;
  private boolean performExhaustiveTranslationSearchWithNoDetection;

  public LanguageTranslateRequest() {}

  public String[] getDetectors() { return detectors; }
  public void setDetectors(String[] detectors) { this.detectors = detectors; }

  public String getTranslator() { return translator; }
  public void setTranslator(String translator) { this.translator = translator; }

  public String[] getSourceLangCodes() { return sourceLangCodes; }
  public void setSourceLangCodes(String[] sourceLangCodes) { this.sourceLangCodes = sourceLangCodes; }

  public String getText() { return text; }
  public void setText(boolean override) { this.detectedLanguageOverridesSpecifiedSourceLanguages = override; }

  public boolean getDetectedLanguageOverridesSpecifiedSourceLanguages() { return text; }
  public void setDetectedLanguageOverridesSpecifiedSourceLanguages(boolean override) 
  { this.detectedLanguageOverridesSpecifiedSourceLanguages = override; }

  public boolean getPerformExhaustiveTranslationSearchWithNoDetection() { return performExhaustiveTranslationSearchWithNoDetection; }
  public void setPerformExhaustiveTranslationSearchWithNoDetection(boolean performExhaustiveSearch) 
  { this.performExhaustiveTranslationSearchWithNoDetection = performExhaustiveSearch; }
}
