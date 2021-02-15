/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.controllers.language;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;
import java.util.HashSet;

import java.net.URLEncoder;
import java.net.URLDecoder;

import org.xml.sax.SAXException;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.Consumes;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.springframework.stereotype.Controller;

import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.ToEnglishTranslator;
import hoot.services.language.LanguageDetector;
import hoot.services.language.LanguageDetectionConsumer;
import hoot.services.language.SupportedLanguage;
import hoot.services.language.SupportedLanguageConsumer;
import hoot.services.language.LanguageAppInfo;
import hoot.services.language.LanguageApp;
import hoot.services.language.LanguageTranslationApp;
import hoot.services.language.LanguageDetectionApp;
import hoot.services.language.LanguageUtils;

import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.commons.lang3.StringUtils;

import javax.annotation.PreDestroy;

/**
 * Web endpoint for language translation and detection

   @todo possibly add some more extensive request param validation
 */
@Controller
@Path("toEnglishTranslation")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws Exception
  {
    try
    {
      //The Joshua init can take a long time, so let's do it here vs having it happen the very 
      //first time a translation is made.
      MethodUtils.invokeStaticMethod(
        Class.forName("hoot.services.language.joshua.JoshuaLanguageTranslator"), "getInstance", null);
    }
    catch (Exception e)
    {
      String msg = "Error initializing Joshua. error: " + e.getMessage();
      logger.error(msg);
      throw new Exception(msg);
    }
  }

  /**
   * This occurs just before the web server shuts down.
   */
  @PreDestroy
  public void preDestroy() throws Exception
  {
    try
    {
      Object joshuaTranslator = 
        MethodUtils.invokeStaticMethod(
          Class.forName("hoot.services.language.joshua.JoshuaLanguageTranslator"), "getInstance", null);
      MethodUtils.invokeMethod(joshuaTranslator, "close");
    }
    catch (Exception e)
    {
      String msg = "Error closing Joshua. error: " + e.getMessage();
      logger.error(msg);
      throw new Exception(msg);
    }
  }

  /**
   * Returns the available language detectors
   *
   * curl localhost:8080/hoot-services/language/detectors
   *
   * {
      "apps":[
      {
         "name":"TikaLanguageDetector",
         "description":"The language detection portion of a library which detects and extracts metadata and text from many different file types",
         "url":"https://tika.apache.org/",
         "supportsConfidence":true
      },
      {
         "name":"OpenNlpLanguageDetector",
         "description":"The language detector portion of a machine learning based toolkit for the processing of natural language text",
         "url":"https://opennlp.apache.org/",
         "supportsConfidence":false
      }
     ]
    }
   */
  @GET
  @Path("/detectors")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageDetectorsResponse getDetectors()
  {
    try
    {
      Set<String> detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      logger.trace("detectorClassNames.size(): " + detectorClassNames.size());
      List<LanguageDetectionApp> apps = new ArrayList<LanguageDetectionApp>();
      for (String detectorClassName : detectorClassNames)
      {
        logger.trace("detectorClassName: " + detectorClassName);
        apps.add((LanguageDetectionApp)languageEntityToApp(detectorClassName));
      }
      logger.trace("apps.size(): " + apps.size());
      return new LanguageDetectorsResponse(apps.toArray(new LanguageDetectionApp[]{}));
    }
    catch (Exception e)
    {
      String msg = 
        "Error retrieving available language detector information.  Error: " + e.getMessage();
      logger.error(msg);
      throw new WebApplicationException(
        e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
    }
  }

  /**
   * Returns the available language translators
   *
   * @todo may want to specify whether a translator supports detection in the response
   *
   * curl localhost:8080/hoot-services/language/translators
   *
   * {
   "apps":[
      {
         "name":"JoshuaLanguageTranslator",
         "description":"A statistical machine translation decoder for phrase-based, hierarchical, and syntax-based machine translation",
         "url":"https://cwiki.apache.org/confluence/display/JOSHUA"
      },
      {
         "name":"HootLanguageTranslator",
         "description":"A custom translator that combines language detection into its workflow.  See the Hootenanny User Documentation for more details.",
         "url":"N/A"
      }
     ]
    }
   */
  @GET
  @Path("/translators")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageTranslatorsResponse getTranslators()
  {
    try
    {
      Set<String> translatorClassNames = ToEnglishTranslatorFactory.getSimpleClassNames();
      logger.trace("translatorClassNames.size(): " + translatorClassNames.size());
      List<LanguageTranslationApp> apps = new ArrayList<LanguageTranslationApp>();
      for (String translatorClassName : translatorClassNames)
      {
        logger.trace("translatorClassName: " + translatorClassName);
        apps.add((LanguageTranslationApp)languageEntityToApp(translatorClassName));
      }
      return new LanguageTranslatorsResponse(apps.toArray(new LanguageTranslationApp[]{}));
    }
    catch (Exception e)
    {
      String msg = 
        "Error retrieving available language translator information.  Error: " + e.getMessage();
      logger.error(msg);
      throw new WebApplicationException(
        e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
    }
  }

  /**
   * Returns information about detectable languages given a collection of language detectors.  

     * If no language detectors are specified, then all available language detectors are used to create the detectable language 
     information.  
     * Language names are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{}' localhost:8080/hoot-services/language/detectable
     curl -X POST -H "Content-Type: application/json" -d '{ "apps": ["TikaLanguageDetector"]}' localhost:8080/hoot-services/language/detectable

     {  
      "languages":[  
      {  
         "name":"Danish",
         "available":true,
         "iso6391code":"da",
         "iso6392code":"dan"
      },
      {  
         "name":"German",
         "available":true,
         "iso6391code":"de",
         "iso6392code":"deu"
      },
     ...
     ]
    }
   */
  @POST
  @Path("/detectable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getDetectableLanguages(SupportedLanguagesRequest request)
  {
    try
    {
      List<String> detectorClassNames = getAppClassNamesFromRequest(request, "detector");
      logger.trace(
        "Listing detectable languages for apps: " + 
        String.join(",", detectorClassNames.toArray(new String[]{})) + "..."); 
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(detectorClassNames));
    }
    catch (Exception e)
    {
      String msg = 
        "Error retrieving detectable language information for apps: " + 
        String.join(",", request.getApps()) + ".  Error: " + e.getMessage();
      logger.error(msg);
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, Response.status(status).entity(msg).build());
    }
  }

  /**
   * Returns information about translatable languages given a collection of language translators.  

     * If no language translators are specified, then all available language translators are used to create the translatable 
     language information.  
     * Language names are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{}' localhost:8080/hoot-services/language/translatable
     curl -X POST -H "Content-Type: application/json" -d '{ "apps": ["JoshuaLanguageTranslator"]}' 
       localhost:8080/hoot-services/language/translatable

     {  
      "languages":[  
      {  
         "name":"Danish",
         "available":false,
         "iso6391code":"da",
         "iso6392code":"dan"
      },
      {  
         "name":"German",
         "available":true,
         "iso6391code":"de",
         "iso6392code":"deu"
      },
     ...
     ]
    }
   */
  @POST
  @Path("/translatable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getTranslatableLanguages(SupportedLanguagesRequest request)
  {
    try
    {
      List<String> translatorClassNames = getAppClassNamesFromRequest(request, "translator");
      logger.trace(
        "Listing translatable languages for apps: " + 
        String.join(",", translatorClassNames.toArray(new String[]{})) + "...");
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(translatorClassNames));
    }
    catch (Exception e)
    {
      String msg = 
        "Error retrieving detectable language information for apps: " + 
        String.join(",", request.getApps()) + ".  Error: " + e.getMessage();
      logger.error(msg);
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, Response.status(status).entity(msg).build());
    }
  }

  /**
   * Detects a spoken language given text and a collection of language detectors.  

     * Detectors will be used in the order specified.  If no language detectors are specified, then as many language detectors as 
       needed are used to perform the detection.  Some detectors may perform better for certain languages than others so there can be
       an advantage to specifying them.
     * The text sent in for translation should be URL encoded and in UTF-8.  
     * Language names and the original source text are returned URL encoded and in UTF-8.

     curl -X POST -H "Content-Type: application/json" -d '{ "text": "wie%20alt%20bist%20du" }' localhost:8080/hoot-services/language/detect
     curl -X POST -H "Content-Type: application/json" -d '{ "detectors": ["TikaLanguageDetector"], "text": "wie%20alt%20bist%20du" }' 
       localhost:8080/hoot-services/language/detect

     {
       "detectorUsed":"TikaLanguageDetector",
       "detectedLangCode":"de",
       "sourceText":"wie%20alt%20bist%20du",
       "detectedLang":"German"
       "detectionConfidence":"high"
     }
   */
  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request)
  {
    String requestText = null;
    try
    {
      requestText = URLDecoder.decode(request.getText(), "UTF-8");
      logger.trace("Detecting language for text: " + StringUtils.left(requestText, 25) + "...");
      String detectedLangCode = "";
      String detectingDetector = "";
      String detectedLangName = "";
      String detectionConfidence = "none";

      List<String> detectorClassNames = getAppClassNamesFromRequest(request, "detector");
      for (String detectorClassName : detectorClassNames)
      {
        if (detectedLangCode.isEmpty())
        {
          logger.trace("detectorClassName: " + detectorClassName);
          LanguageDetector detector = LanguageDetectorFactory.create(detectorClassName);
          detectingDetector = detectorClassName;
          logger.trace("detectingDetector: " + detectorClassName);
          detectedLangCode = detector.detect(requestText);
          logger.trace("detectedLangCode: " + detectedLangCode);
          if (!detectedLangCode.isEmpty()) 
          {
            detectedLangName = 
              ((SupportedLanguageConsumer)detector).getLanguageName(detectedLangCode);
            logger.trace("detectedLangName: " + detectedLangName);
            if (StringUtils.trimToNull(detectedLangName) == null)
            {
              //Some detectors seem to detect more languages than what they advertise (Tika),
              //so doing this for now to avoid an error.  Later, can find all instances of an 
              //unavailable lang name and update the appropriate config files.
              detectedLangName = "unavailable";
            }
            detectionConfidence = LanguageUtils.confidenceToString(detector.getConfidence());
            logger.trace("detectionConfidence: " + detectionConfidence);
          }
        }
      }

      JSONObject entity = new JSONObject();
      entity.put("sourceText", request.getText());
      entity.put("detectedLangCode", detectedLangCode);
      if (!detectedLangCode.isEmpty())
      {
        entity.put("detectedLang", encodeText(detectedLangName));
        entity.put("detectorUsed", detectingDetector);
        entity.put("detectionConfidence", detectionConfidence);
      } 
      logger.trace(entity.toJSONString());
      return Response.ok(entity.toJSONString()).build();
    }
    catch (Exception e)
    {
      String msg = 
        "Error detecting language with detector(s): " + 
        String.join(",", request.getDetectors()) + "; error: " + e.getMessage() + 
        " text: " + request.getText();
      logger.error(msg);
      Status status;
      if (e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(msg).build());
    }
  }

  /**
   * Translates a single line of text given a specified language translator and one or more 
     source languages.  

     * When using JoshuaLanguageTranslator, an associated translation service must be running 
       for each specified source language.  See the Installation Guide for more details.
     * In place of language codes, some translators support specifying "detect", which will 
       cause the translator to attempt to detect the language before translating 
       (e.g. HootLanguageTranslator).  
     * The text sent in for translation should be URL encoded and in UTF-8.  
     * The translated text is returned URL encoded and in UTF-8.  
     * For translators that support detection, detectors will be used in the order 
       specified.  If no language detectors are specified, then as many language detectors as 
       needed are used to perform the detection.  Some detectors may perform better for certain 
       languages than others so there can be an advantage to specifying them.
     * If the specified translator does not support detection, then any request parameters 
       related to detection will be ignored.

     curl -X POST -H "Content-Type: application/json" -d '{ "translator": "JoshuaLanguageTranslator", "sourceLangCodes": ["de"], 
       "text": "wie%20alt%20bist%20du" }' localhost:8080/hoot-services/language/translate

     {
       "translatedText":"How%20old%20are%20you",
       "performExhaustiveTranslationSearchWithNoDetection":false,
       "sourceLangCodes":[
         "de"
       ],
       "translator":"JoshuaLanguageTranslator",
       "sourceText":"wie%20alt%20bist%20du",
       "detectedLanguageOverridesSpecifiedSourceLanguages":false
     }
   */
  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    return translate(request, false);
  }

  /**
   * Translates multiple lines of text in batch given a specified language translator and one or more 
     source languages.  

     * Only a single source language is supported.
     * When using JoshuaLanguageTranslator, an associated translation service must be the 
       specified source language.  See the Installation Guide for more details.
     * No translator may be used that is integrated with language detection when translating in 
       batch (e.g. HootLanguageTranslator).  
     * The text sent in for translation should have one line for each piece of text to be translated 
       with all of text URL encoded and in UTF-8.  
     * The translated text is returned translated line by line, URL encoded, and in UTF-8. 
     * For translators that support detection, detectors will be used in the order specified.  If 
       no language detectors are specified, then as many language detectors as needed are used to 
       perform the detection.  Some detectors may perform better for certain languages than others 
       so there can be an advantage to specifying them. 
     * An error will be thrown if a translator is specified that supports language detection.

     curl -X POST -H "Content-Type: application/json" -d '{ "translator": "JoshuaLanguageTranslator", "sourceLangCodes": ["de"], 
       "text": "wie%20alt%20bist%20du\nwie%20heissen%20sie" }' localhost:8080/hoot-services/language/translateBatch

     {
       "translatedText":"How%20old%20are%20you%0AWhat%27s%20your%20name%0A",
       "performExhaustiveTranslationSearchWithNoDetection":false,
       "sourceLangCodes":[
         "de"
       ],
       "translator":"JoshuaLanguageTranslator",
       "sourceText":"wie%20alt%20bist%20du\nwie%20heissen%20sie",
       "detectedLanguageOverridesSpecifiedSourceLanguages":false
     }
   */
  @POST
  @Path("/translateBatch")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translateBatch(LanguageTranslateRequest request) 
  {
    return translate(request, true);
  }

  private Response translate(LanguageTranslateRequest request, boolean batch)
  {
    String textToTranslate = null;
    String responseStr = null;
    boolean badRequest = false;
    try
    {
      //could make this check more robust if many translators are added
      if (batch && request.getTranslator().toLowerCase().equals("hootlanguagetranslator"))
      {
        badRequest = true;
        throw new Exception("HootLanguageTranslator may not be used in batch translation.");
      }
      if (batch && request.getSourceLangCodes().length > 1)
      {
        badRequest = true;
        throw new Exception("Batch translation only supports one source language.");
      }

      textToTranslate = URLDecoder.decode(request.getText(), "UTF-8");
      if (!batch)
      {
        //This isn't batch translation, so remove all newlines and translate this as a single 
        //piece of text.
        textToTranslate = textToTranslate.replaceAll("\n", "");
      }
      logger.debug(
        "Translating language for text: " + StringUtils.left(textToTranslate, 25) + 
        " and source languages: " + String.join(",", request.getSourceLangCodes()) + "...");

      ToEnglishTranslator translator = ToEnglishTranslatorFactory.create(request.getTranslator());
      translator.setConfig(request);
      String translatedText = translator.translate(request.getSourceLangCodes(), textToTranslate);
      responseStr = toTranslateResponse(request, translatedText, translator);
    }
    catch (Exception e)
    {
      String msg = 
        "Error translating with translator: " + request.getTranslator() + " to language(s): " + 
        String.join(",", request.getSourceLangCodes()) + ".  Error: " + e.getMessage() + 
        "; text: " + textToTranslate;
      logger.error(msg);
      Status status;
      if (badRequest || e instanceof IllegalArgumentException)
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, Response.status(status).entity(msg).build());
    }
    
    return Response.ok(responseStr).build();
  }

  /*
   * Instantiates a language app
   */
  private LanguageApp languageEntityToApp(String appName) throws Exception
  {
    LanguageAppInfo appInfo = null;
    LanguageApp app = null;
    if (ToEnglishTranslatorFactory.getSimpleClassNames().contains(appName))
    {
      appInfo = (LanguageAppInfo)ToEnglishTranslatorFactory.create(appName);
      app = new LanguageTranslationApp();
    }
    else
    {
      appInfo = (LanguageAppInfo)LanguageDetectorFactory.create(appName);
      app = new LanguageDetectionApp();
    }
    assert(appInfo != null);
    logger.trace(appInfo.getDescription());
    logger.trace(appInfo.getUrl());

    app.setName(appName);
    app.setDescription(appInfo.getDescription());
    app.setUrl(appInfo.getUrl());
    if (app instanceof LanguageDetectionApp)
    {
      LanguageDetectionApp detectionApp = (LanguageDetectionApp)app;
      detectionApp.setSupportsConfidence(appName.equals("TikaLanguageDetector"));
    }
    return app;
  }

  /*
   * Returns all unique supported languages, given the set of specified apps
   */
  private SupportedLanguage[] getAllAppSupportedLangs(List<String> apps) throws Exception
  {
    Set<String> parsedLangCodes = new HashSet<String>();
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    for (String appName : apps)
    {
      logger.trace("appName: " + appName);
      SupportedLanguageConsumer langConsumer = null;
      if (ToEnglishTranslatorFactory.getSimpleClassNames().contains(appName))
      {
        langConsumer = (SupportedLanguageConsumer)ToEnglishTranslatorFactory.create(appName);
      }
      else
      {
        langConsumer = (SupportedLanguageConsumer)LanguageDetectorFactory.create(appName);
      }
      assert(langConsumer != null);

      SupportedLanguage[] consumerSupportedLangs = langConsumer.getSupportedLanguages();
      logger.trace("consumerSupportedLangs size: " + consumerSupportedLangs.length);
      for (int i = 0; i < consumerSupportedLangs.length; i++)
      {
        SupportedLanguage lang = consumerSupportedLangs[i];
        logger.trace("lang code: " + lang.getIso6391Code());
        if (!parsedLangCodes.contains(lang.getIso6391Code()))
        {
          parsedLangCodes.add(lang.getIso6391Code());
          lang.setAvailable(langConsumer.isLanguageAvailable(lang.getIso6391Code()));
          logger.trace("detectedLangName: " + lang.getName());
          logger.trace("encoded detectedLangName: " + encodeText(lang.getName()));
          lang.setName(encodeText(lang.getName()));
          supportedLangs.add(lang);
        }
      }
    }
    logger.trace("supportedLangs size: " + supportedLangs.size());
    return supportedLangs.toArray(new SupportedLanguage[]{});
  }

  private String toTranslateResponse(LanguageTranslateRequest request, String translatedText, 
    ToEnglishTranslator translator) throws UnsupportedEncodingException
  {
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    JSONArray sourceLangCodes = new JSONArray();
    for (int i = 0; i < request.getSourceLangCodes().length; i++)
    {
      sourceLangCodes.add(request.getSourceLangCodes()[i]);
    }
    entity.put("sourceLangCodes", sourceLangCodes);
    entity.put("translatedText", encodeText(translatedText));
    entity.put("translator", request.getTranslator());
    if (translator instanceof LanguageDetectionConsumer)
    {
      LanguageDetectionConsumer detectionConsumer = (LanguageDetectionConsumer)translator;
      if (detectionConsumer.getDetectedLangCode() != null && 
          !detectionConsumer.getDetectedLangCode().isEmpty())
      {
        entity.put("detectedLangCode", detectionConsumer.getDetectedLangCode());
        entity.put(
          "detectedLang", 
          ((SupportedLanguageConsumer)translator).getLanguageName(
            detectionConsumer.getDetectedLangCode()));
        entity.put("detectorUsed", detectionConsumer.getDetectorUsed());
        entity.put(
          "detectedLangAvailableForTranslation", 
          ((SupportedLanguageConsumer)translator).isLanguageAvailable(
            detectionConsumer.getDetectedLangCode()));
      }
    }
    entity.put(
      "detectedLanguageOverridesSpecifiedSourceLanguages", 
      request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    entity.put(
      "performExhaustiveTranslationSearchWithNoDetection", 
      request.getPerformExhaustiveTranslationSearchWithNoDetection());
    return entity.toJSONString();
  }

  /*
   * Given a request that specifies an app (detector, translator, etc.) and returns all classes 
     of that type of app.  We want to preserve the order of the apps passed in 
   */
  private List<String> getAppClassNamesFromRequest(Object request, String type) throws Exception
  {
    List<String> appClassNames = new ArrayList<String>();

    //this could maybe be simplified with some refactoring
    if (request instanceof SupportedLanguagesRequest)
    {
      SupportedLanguagesRequest supportedLangsRequest = (SupportedLanguagesRequest)request;
      if (supportedLangsRequest.getApps() != null && supportedLangsRequest.getApps().length > 0)
      {  
        appClassNames = Arrays.asList(supportedLangsRequest.getApps());
      }
    }
    else if (request instanceof LanguageDetectRequest)
    {
      LanguageDetectRequest langDetectRequest = (LanguageDetectRequest)request;
      if (langDetectRequest.getDetectors() != null && langDetectRequest.getDetectors().length > 0)
      {  
        appClassNames = Arrays.asList(langDetectRequest.getDetectors());
      }
    }
    else
    {
      throw new Exception("Invalid request type.");
    }
    
    //If no apps were specified, the returned app ordering is at the mercy of the reflection.
    if (appClassNames.isEmpty())
    {
      if (type.equals("detector"))
      {
        appClassNames.addAll(LanguageDetectorFactory.getSimpleClassNames());
      }
      else
      {
        appClassNames.addAll(ToEnglishTranslatorFactory.getSimpleClassNames());
      }
    }

    return appClassNames;
  }

  public static String encodeText(String text) throws UnsupportedEncodingException
  { 
    //the replace here is a bit of a hack...
    return URLEncoder.encode(text, "UTF-8").replace("+", "%20");
  }
}
