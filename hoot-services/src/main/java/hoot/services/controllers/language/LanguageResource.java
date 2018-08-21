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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.commons.lang3.StringUtils;

import javax.annotation.PreDestroy;

/*
 * 
 */
@Controller
@Path("")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws Exception
  {
    try
    {
      //The Joshua init can take a long time, so let's do it here vs having it happen the very first time a translation is made.
      logger.error("Initializing Joshua...");
      MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
    }
    catch (Exception e)
    {
      throw new Exception("Error initializing Joshua. error: " + e.getMessage());
    }
  }

  @PreDestroy
  public void preDestroy() throws Exception
  {
    try
    {
      logger.error("Closing Joshua...");
      Object joshuaTranslator = 
        MethodUtils.invokeStaticMethod(Class.forName("hoot.services.language.JoshuaLanguageTranslator"), "getInstance", null);
      if (joshuaTranslator == null)
      {
        logger.error("joshuaTranslator null");
      }
      MethodUtils.invokeMethod(joshuaTranslator, "close");
    }
    catch (Exception e)
    {
      throw new Exception("Error closing Joshua. error: " + e.getMessage());
    }
  }

  @GET
  @Path("/detectors")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageAppsResponse getDetectors()
  {
    try
    {
      Set<String> detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      List<LanguageApp> apps = new ArrayList<LanguageApp>();
      for (String detectorClassName : detectorClassNames)
      {
        apps.add(languageEntityToApp(detectorClassName));
      }
      return new LanguageAppsResponse(apps.toArray(new LanguageApp[]{}));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
         .entity("Error retrieving available language detector information.  Error: " + e.getMessage())
         .build());
    }
  }

  @GET
  @Path("/translators")
  @Produces(MediaType.APPLICATION_JSON)
  public LanguageAppsResponse getTranslators()
  {
    try
    {
      Set<String> translatorClassNames = ToEnglishTranslatorFactory.getSimpleClassNames();
      List<LanguageApp> apps = new ArrayList<LanguageApp>();
      for (String translatorClassName : translatorClassNames)
      {
        apps.add(languageEntityToApp(translatorClassName));
      }
      return new LanguageAppsResponse(apps.toArray(new LanguageApp[]{}));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
         .entity("Error retrieving available language translator information.  Error: " + e.getMessage())
         .build());
    }
  }

  private LanguageApp languageEntityToApp(String appName) throws Exception
  {
    LanguageAppInfo appInfo = null;
    try
    {
      appInfo = (LanguageAppInfo)ToEnglishTranslatorFactory.create(appName);
    }
    catch (Exception e)
    {
      appInfo = (LanguageAppInfo)LanguageDetectorFactory.create(appName);
    }
    assert(appInfo != null);

    LanguageApp app = new LanguageApp();
    app.setName(appName);
    app.setDescription(appInfo.getDescription());
    app.setUrl(appInfo.getUrl());
    return app;
  }

  @POST
  @Path("/detectable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getDetectableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      Set<String> detectorClassNames = new HashSet<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        detectorClassNames = new HashSet<String>(Arrays.asList(request.getApps()));
      }
      if (detectorClassNames.isEmpty())
      {
        detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      }
      logger.error("Listing detectable languages for apps: " + String.join(",", detectorClassNames.toArray(new String[]{})) + "..."); 
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(detectorClassNames));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving detectable language information.  Error: " + e.getMessage())
          .build());
    }
  }

  @POST
  @Path("/translatable")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedLanguagesResponse getTranslatableLangs(SupportedLanguagesRequest request)
  {
    try
    {
      Set<String> translatorClassNames = new HashSet<String>();
      if (request.getApps() != null && request.getApps().length > 0)
      {  
        translatorClassNames = new HashSet<String>(Arrays.asList(request.getApps()));
      }
      if (translatorClassNames.isEmpty())
      {
        translatorClassNames = ToEnglishTranslatorFactory.getSimpleClassNames();
      }
      logger.error("Listing translatable languages for apps: " + String.join(",", translatorClassNames.toArray(new String[]{})) + "...");
      return new SupportedLanguagesResponse(getAllAppSupportedLangs(translatorClassNames));
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving translatable language information.  Error: " + e.getMessage())
          .build());
    }
  }

  private SupportedLanguage[] getAllAppSupportedLangs(Set<String> apps) throws Exception
  {
    Set<String> parsedLangCodes = new HashSet<String>();
    List<SupportedLanguage> supportedLangs = new ArrayList<SupportedLanguage>();
    for (String appName : apps)
    {
      logger.error("appName: " + appName);
      SupportedLanguageConsumer langConsumer = null;
      try
      {
        langConsumer = (SupportedLanguageConsumer)ToEnglishTranslatorFactory.create(appName);
      }
      catch (Exception e)
      {
        langConsumer = (SupportedLanguageConsumer)LanguageDetectorFactory.create(appName);
      }
      assert(langConsumer != null);

      SupportedLanguage[] consumerSupportedLangs = langConsumer.getSupportedLanguages();
      logger.error("consumerSupportedLangs size: " + consumerSupportedLangs.length);
      for (int i = 0; i < consumerSupportedLangs.length; i++)
      {
        SupportedLanguage lang = consumerSupportedLangs[i];
        logger.trace("lang code: " + lang.getIso6391code());
        if (!parsedLangCodes.contains(lang.getIso6391code()))
        {
          parsedLangCodes.add(lang.getIso6391code());
          lang.setAvailable(langConsumer.isLanguageAvailable(lang.getIso6391code()));
          supportedLangs.add(lang);
        }
      }
    }
    logger.error("supportedLangs size: " + supportedLangs.size());
    return supportedLangs.toArray(new SupportedLanguage[]{});
  }

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
      logger.error("Detecting language for text: " + StringUtils.left(requestText, 25) + "...");
      String detectedLangCode = "";
      String detectingDetector = "";
      String detectedLangName = "";
      Set<String> detectorClassNames = new HashSet<String>();
      if (request.getDetectors() != null && request.getDetectors().length > 0)
      {  
        detectorClassNames = new HashSet<String>(Arrays.asList(request.getDetectors()));
      }
      if (detectorClassNames.isEmpty())
      {
        detectorClassNames = LanguageDetectorFactory.getSimpleClassNames();
      }

      for (String detectorClassName : detectorClassNames)
      {
        if (detectedLangCode.isEmpty())
        {
          LanguageDetector detector = LanguageDetectorFactory.create(detectorClassName);
          detectingDetector = detectorClassName;
          detectedLangCode = detector.detect(requestText);
          if (!detectedLangCode.isEmpty())
          {
            detectedLangName = ((SupportedLanguageConsumer)detector).getLanguageName(detectedLangCode);
          }
        }
      }

      JSONObject entity = new JSONObject();
      entity.put("sourceText", requestText);
      entity.put("detectedLangCode", detectedLangCode);
      if (!detectedLangCode.isEmpty())
      {
        entity.put("detectedLang", detectedLangName);
        entity.put("detectorUsed", detectingDetector);
      } 
      return Response.ok(entity.toJSONString()).build();
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, 
        Response.status(Status.INTERNAL_SERVER_ERROR)
          .entity(
            "Error detecting language with detector(s): " + String.join(",", request.getDetectors()) + "; error: " + e.getMessage() + 
            " text: " + request.getText())
          .build());
    }
  }

  private String toTranslateResponse(LanguageTranslateRequest request, String translatedText, ToEnglishTranslator translator) 
    throws UnsupportedEncodingException
  {
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    JSONArray sourceLangCodes = new JSONArray();
    for (int i = 0; i < request.getSourceLangCodes().length; i++)
    {
      sourceLangCodes.add(request.getSourceLangCodes()[i]);
    }
    entity.put("sourceLangCodes", sourceLangCodes);
    //TODO: replace of '+' here is a bit of a hack
    entity.put("translatedText", URLEncoder.encode(translatedText, "UTF-8").replace("+", "%20"));
    entity.put("translator", request.getTranslator());
    if (translator instanceof LanguageDetectionConsumer)
    {
      LanguageDetectionConsumer detectionConsumer = (LanguageDetectionConsumer)translator;
      if (detectionConsumer.getDetectedLangCode() != null && !detectionConsumer.getDetectedLangCode().isEmpty())
      {
        entity.put("detectedLangCode", detectionConsumer.getDetectedLangCode());
        entity.put("detectedLang", ((SupportedLanguageConsumer)translator).getLanguageName(detectionConsumer.getDetectedLangCode()));
        entity.put("detectorUsed", detectionConsumer.getDetectorUsed());
        entity.put(
          "detectedLangAvailableForTranslation", 
          ((SupportedLanguageConsumer)translator).isLanguageAvailable(detectionConsumer.getDetectedLangCode()));
      }
    }
    entity.put("detectedLanguageOverridesSpecifiedSourceLanguages", request.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    entity.put("performExhaustiveTranslationSearchWithNoDetection", request.getPerformExhaustiveTranslationSearchWithNoDetection());
    return entity.toJSONString();
  }

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    String textToTranslate = null;
    String responseStr = null;
    try
    {
      textToTranslate = URLDecoder.decode(request.getText(), "UTF-8");
      logger.error(
        "Translating language for text: " + StringUtils.left(textToTranslate, 25) + " and source languages: " + 
        String.join(",", request.getSourceLangCodes()) + "...");

      ToEnglishTranslator translator = ToEnglishTranslatorFactory.create(request.getTranslator());
      translator.setConfig(request);
      String translatedText = translator.translate(request.getSourceLangCodes(), textToTranslate);

      responseStr = toTranslateResponse(request, translatedText, translator);
    }
    catch (Exception e)
    {
      Status status;
      //TODO: handle more exceptions
      if (e.getMessage().startsWith("No language translator available") || 
          e.getMessage().startsWith("Requested unsupported translation language"))
      {
        status = Status.BAD_REQUEST;
      }
      else
      {
        status = Status.INTERNAL_SERVER_ERROR;
      } 
      throw new WebApplicationException(
        e, 
        Response.status(status)
          .entity(
            "Error translating with translator: " + request.getTranslator() + " to language (s): " + 
            String.join(",", request.getSourceLangCodes()) + ".  Error: " + 
            e.getMessage() + "; text: " + textToTranslate)
          .build());
    }
    
    return Response.ok(responseStr).build();
  }
}
