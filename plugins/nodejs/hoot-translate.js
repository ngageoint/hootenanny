const debug = require('debug')('hoot-translate');
var mgcp = require('./mgcp_translations.js');

config = require('./config.js');
// Make sure the MGCP translator exports extra tags to the TXT field
config.OgrMgcpExtra = 'note';
// Throw errors instead of returning partial translations/o2s_X features
config.OgrThrowError = true;

module.exports = {
    mgcp: mgcp
}