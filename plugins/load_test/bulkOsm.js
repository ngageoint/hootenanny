var fs = require('fs');

module.exports = {
  loadBulkOsm: loadBulkOsm,
  alertTranslation: alertTranslation
}

function loadBulkOsm(requestParams, context, ee, next) {
  context.vars.bulkFile = fs.readFileSync('./' + context.vars.bulkFile).toString();
  return next();
}

function alertTranslation(requestParams, context, ee, next) {
  console.log('translated')
  return next();
}