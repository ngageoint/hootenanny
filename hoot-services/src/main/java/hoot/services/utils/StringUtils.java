package hoot.services.utils;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

/**
 * String utilities
 */
public class StringUtils
{
	/**
	 * URI encodes a string for Javascript consumption; client should call decodeURIComponent to 
	 * decode
	 * 
	 * @param str string to encode
	 * @return a URI encoded string safe for Javascript consumption
	 * @throws UnsupportedEncodingException 
	 */
	public static String encodeURIComponentForJavaScript(final String str) 
		throws UnsupportedEncodingException
	{
		return 
			URLEncoder
			  .encode(str, "UTF-8")
			  .replaceAll("\\+", "%20")
		    .replaceAll("\\%21", "!")
		    .replaceAll("\\%27", "'")
		    .replaceAll("\\%28", "(")
		    .replaceAll("\\%29", ")")
		    .replaceAll("\\%7E", "~")
		    .replaceAll("\\%3B", ";");
	}
}
