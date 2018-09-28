const proxy = require('express-http-proxy');
const app   = require('express')();
const argv  = require('minimist')(process.argv.slice(2));
const TOMCAT_PORT = argv.tomcat_port || process.env.TOMCAT_PORT || 8080;

app.use('/', proxy(`localhost:${TOMCAT_PORT}`, {
    proxyReqOptDecorator: function(proxyReqOpts, srcReq) {
        proxyReqOpts.headers['cookie'] = 'SESSION=ff47f751-c831-41ee-800f-5ef8b9371ee3; lock=1';
        return proxyReqOpts;
    }
}));

app.listen(9090);