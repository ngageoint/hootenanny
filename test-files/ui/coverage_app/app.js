var coverage = require('istanbul-middleware');
var express = require('express');
var app = express();

/* Configure CORS. */

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers",
                    "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.use('/coverage', coverage.createHandler());

app.listen(8880);