
var __extension__ = "";

var hoot = {};

hoot.require = function(param)
{
}

function require(param)
{	

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
