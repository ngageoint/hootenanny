var mgcp = require('./mgcp.js');
var emgcp = require('./emgcp.js');
var emgcp_osm = require('./emgcp_osm.js');

module.exports = {
    EnglishtoOSM: emgcp_osm.toOSM,
    RawtoOSM: mgcp.toOsm,
    OSMtoEnglish: emgcp.toEnglish,
    OSMtoRaw: mgcp.toMgcp
}
