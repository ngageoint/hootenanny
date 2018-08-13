
package hoot.services.controllers.language;

import java.io.IOException;
import java.util.List;
import java.util.Arrays;

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
import hoot.services.language.SupportedLanguagesReader;
//import hoot.services.language.JoshuaLanguageTranslator;

/*
 * 
 */
@Controller
@Path("")
public class LanguageResource 
{
  private static final Logger logger = LoggerFactory.getLogger(LanguageResource.class);

  public LanguageResource() throws IOException
  {
    //Joshua init takes a long time - TODO: temp?
    //JoshuaLanguageTranslator.getInstance();
  }

  @GET
  @Path("/supportedLangs")
  @Produces(MediaType.APPLICATION_JSON)
  public SupportedTranslationLanguagesResponse getSupportedLangs()
  {
    try
    {
      return new SupportedTranslationLanguagesResponse(SupportedLanguagesReader.getInstance().getSupportedLanguages());
    }
    catch (Exception e)
    {
      throw new WebApplicationException(
        e, Response.status(Status.INTERNAL_SERVER_ERROR).entity("Error retrieving supported language information").build());
    }
  }

  @POST
  @Path("/detect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response detectLanguage(LanguageDetectRequest request)
  {
    try
    {
      logger.debug(String.join(",", request.getDetectors()));
      logger.debug(request.getText());

      String detectedLang = "";
      String detectingDetector = "";
      List<String> detectors = Arrays.asList(request.getDetectors());
      if (detectors.isEmpty())
      {
        //TODO: add these with reflection
        detectors.add("TikaLanguageDetector");
        detectors.add("OpenNlpLanguageDetector");
      }

      for (String detector : detectors)
      {
        if (detectedLang.isEmpty())
        {
          detectedLang = LanguageDetectorFactory.create(detector).detect(request.getText());
        }
      }

      JSONObject entity = new JSONObject();
      entity.put("sourceText", request.getText());
      entity.put("detectedLang", detectedLang);
      entity.put("detectingDetector", detectingDetector);
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

  @POST
  @Path("/translate")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateRequest request) 
  {
    logger.debug(request.getTranslator());
    logger.debug(request.getSourceLangCode());
    logger.debug(request.getText());

    String translatedText = "";
    try
    {
      if (!SupportedLanguagesReader.getInstance().isSupportedLanguage(request.getSourceLangCode()))
      {
        throw new WebApplicationException(
          Response.status(Status.BAD_REQUEST).entity("Requested unsupported translation language: " + request.getSourceLangCode()).build());
      }

      translatedText = 
        ToEnglishTranslatorFactory.create(request.getTranslator()).translate(request.getSourceLangCode(), request.getText());
    }
    catch (Exception e)
    {
      Status status;
      if (e.getMessage().startsWith("No language translator available"))
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
            "Error translating with translator: " + request.getTranslator() + " to language: " + request.getSourceLangCode() + "; text: " + 
            request.getText())
          .build());
    }
    
    JSONObject entity = new JSONObject();
    entity.put("sourceText", request.getText());
    entity.put("translatedText", translatedText);
    entity.put("translator", request.getTranslator());
    return Response.ok(entity.toJSONString()).build();
  }

  /*@POST
  @Path("/translateWithDetect")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translate(LanguageTranslateWithDetectRequest request) 
  {
    
  }*/
}
