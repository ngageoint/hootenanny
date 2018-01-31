var fs = require('fs');

module.exports = {
  loadBulkOsm: loadBulkOsm,
  alertTranslation: alertTranslation
}

function loadBulkOsm(requestParams, context, ee, next) {
  context.vars.osmBody = '"' + fs.readFileSync('./' + context.vars.osmBody).toString() + '"';
  console.log(contex.vars.osmBody)
  return next();
}

function alertTranslation(requestParams, context, ee, next) {
  console.log('translated')
  return next();
}