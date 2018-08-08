//package hoot.core.language;

import java.io.IOException;

import org.apache.tika.langdetect.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageDetector;
import org.apache.tika.language.detect.LanguageResult;

import org.apache.tika.exception.TikaException;

import org.xml.sax.SAXException;

public class TikaLanguageDetector 
{
  //TODO: may want to consider running this as service, so as not to incur model load time with every call
  //TODO: Tika has some other supported language detectors we could try...
  public static void main(String args[]) throws IOException, SAXException, TikaException
  {
    System.out.println((new OptimaizeLangDetector().loadModels()).detect(args[0]).getLanguage());
  }
}
