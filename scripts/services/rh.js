
var __extension__ = "";

var hoot = {};

hoot.require = function(param)
{	
	// var root_path = APP_ROOT + "/plugins/script";
	var root_path = APP_ROOT + "/plugins/";
	// var path = "";
	// var res = param.split("."); 
	// for(var i=0; i<res.length; i++)
	// {
	// 	path += "/" + res[i];
	// }
	// path += "/__init__.js";
	// var headerpath = root_path + path;

	var headerpath = root_path + param + ".js";

	//engine.eval(new java.io.FileReader(path));
	/*try{	
		context.evaluateReader(scope, new java.io.BufferedReader(new java.io.FileReader(headerpath)), param, 1, null);
	}
	catch(err)
	{
		// try plugin-local folder which contains fouo data
		root_path = APP_ROOT + "/plugins-local/script";
		headerpath = root_path + path;
		context.evaluateReader(scope, new java.io.BufferedReader(new java.io.FileReader(headerpath)), param, 1, null);
	}*/
RH_JS_FILEREADER = new java.io.FileReader(headerpath);
RH_JS_BUFFEREDREADER = new java.io.BufferedReader(RH_JS_FILEREADER);
context.evaluateReader(scope, RH_JS_BUFFEREDREADER, param, 1, null);
RH_JS_FILEREADER.close();
RH_JS_BUFFEREDREADER.close();
}

function require(param)
{	
	// var root_path = APP_ROOT + "/plugins/script";
	var root_path = APP_ROOT + "/plugins/";
	// var path = "";
	// var res = param.split("."); 
	// for(var i=0; i<res.length; i++)
	// {
	// 	path += "/" + res[i];
	// }
	// path += "/__init__.js";
	// var headerpath = root_path + path;

	var headerpath = root_path + param + ".js";

	//engine.eval(new java.io.FileReader(path));
	/*try{	
		context.evaluateReader(scope, new java.io.BufferedReader(new java.io.FileReader(headerpath)), param, 1, null);
	}
	catch(err)
	{
		// try plugin-local folder which contains fouo data
		root_path = APP_ROOT + "/plugins-local/script";
		headerpath = root_path + path;
		context.evaluateReader(scope, new java.io.BufferedReader(new java.io.FileReader(headerpath)), param, 1, null);
	}*/
RH_JS_FILEREADER = new java.io.FileReader(headerpath);
RH_JS_BUFFEREDREADER = new java.io.BufferedReader(RH_JS_FILEREADER);
context.evaluateReader(scope, RH_JS_BUFFEREDREADER, param, 1, null);
RH_JS_FILEREADER.close();
RH_JS_BUFFEREDREADER.close();
}

function __setupPackage__(param)
{
}

function getHootConfig(param)
{
  return false;
}

function createUuid()
{// temporary should be replaced by java code
  /*  var s = [];
    var hexDigits = "0123456789ABCDEF";
    for (var i = 0; i < 32; i++) {
        s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
   
    // bits 12-15 of the time_hi_and_version field to 0010
    s[12] = "4"; 
    // bits 6-7 of the clock_seq_hi_and_reserved to 01
    s[16] = hexDigits.substr((s[16] & 0x3) | 0x8, 1); 
    var uuid = s.join("");
    return uuid;*/
  return "" + java.util.UUID.randomUUID().toString();
}

function logError(msg)
{
}

function logWarn(msg)
{
}
