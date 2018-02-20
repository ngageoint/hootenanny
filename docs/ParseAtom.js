
xml2js = require('xml2js');
htmlparser = require('htmlparser');
https = require('https');
fs = require('fs');


var titlePattern = new RegExp(/^Requirement: (.*)$/);
var requirementPattern = new RegExp(/^([0-9\.]+)\s+.+$/);
var statusPattern = new RegExp(/^Requirement:.+\((.*)\): .*$/);

function getTitle(str)
{
    var result = str.match(/^Requirement #[0-9]+ \(.+\): (.*)$/);
    return result[1];
}

var req = {};

var handler = new htmlparser.DefaultHandler(function(error, dom) {
    req.text = h2aChildren(dom);
});
var parser = new htmlparser.Parser(handler);

function isRequirement(str)
{
    var match = str.match(requirementPattern);
    return match != undefined;
}

function parseStatus(str)
{
    var match = str.match(/^Requirement #[0-9]+ \((.+)\): .*$/);
    var status = match[1];
    return status;
}

function parseTicket(str)
{
    var match = str.match(/^Requirement #([0-9]+) \(.+\): .*$/);
    return match[1];
}

function toAsciiDoc(req, h)
{
    if (h == undefined)
    {
        req.text = '';
        return;
    }
    parser.parseComplete("<html>" + h + "</html>");
}

function h2aChildren(h)
{
    var result = '';
    if (typeof(h) == 'string')
    {
        result = h;
    }
    else if (h instanceof Array)
    {
        for (k in h)
        {
            result = result + h2aChildren(h[k]);
        }
    }
    else
    {
        if (h.type == 'tag')
        {
            result = result + h2a[h.name](h.children, h.attribs);
        }
        else if (h.type == 'text')
        {
            if (isRequirement(h.data) && req.title == undefined)
            {
                req.title = h.data;
            }
            else
            {
                result = result + h.data;
            }
        }
    }

    return result;
}

h2a = {
    'a': function(h, attribs) {
        return attribs.href + "[" + h2aChildren(h) + "]";
    },
    'blockquote': function(h) { return '[quote]\n____\n' + h2aChildren(h) + '____\n'; },
    'br': function(h) { return "\n"; },
    'code': function(h) { return "+" + h2aChildren(h) + "+"; },
    'del': function(h) { return "*REMOVED:* _" + h2aChildren(h) + "_"; },
    'em': function(h) { return "_" + h2aChildren(h) + "_"; },
    'hr': function(h) { return "\n'''\n"; },
    'html': function(h) { return h2aChildren(h); },
    'p': function(h) { return '\n' + h2aChildren(h) + '\n'; },
    'i': function(h) { return '_' + h2aChildren(h) + '_'; },
    'li': function(h) { return '* ' + h2aChildren(h); },
    'strong': function(h) { return "*" + h2aChildren(h) + "*"; },
    'table': function() { return " *Please don't use tables in requirements* "; },
    'ul': function(h) { return h2aChildren(h); },
}

console.log('= ICE Phase II - Requirements Status');
console.log('https://github.com/ngageoint/hootenanny');
var d = new Date();
console.log((d.getMonth() + 1) + '/' + d.getDate() + '/' + d.getFullYear());
console.log();
console.log('== Introduction');
console.log();
console.log("Below is an understanding of the requirements as listed in the _National Geospatial-Intelligence Agency (NGA) Statement of Work (SOW) for Enhanced Conflation Web-Services for Foundation GEOINT Content Management, Phase 2, 03 June 2014, Version 0.3_. This is an attempt to further define the requirements and increase mutual understanding. The Statement of Work is the authoritative document.");
console.log();
console.log("Estimates that are in *bold* represent the sum of the child estimates.");
console.log();

function parseReqNumber(str)
{
    var match = str.match(/^([0-9\.]+)\.\s.*$/);
    if (match == null)
    {
        return [];
    }
    var numStr = match[1].split('.');
    var num = [];
    for (k in numStr)
    {
        num.push(Number(numStr[k]));
    }
    return num;
}

function compareReq(a, b)
{
    var an = parseReqNumber(a.title);
    var bn = parseReqNumber(b.title);
    for (var i = 0; i < an.length && i < bn.length; i++)
    {
        if (an[i] < bn[i])
        {
            return -1;
        }
        else if (an[i] > bn[i])
        {
            return 1;
        }
    }

    if (an.length < bn.length)
    {
        return -1;
    }
    else if (an.length > bn.length)
    {
        return 1;
    }
    
    // if the requirement numbers are the same or non-existant, compare based
    // on strings
    if (a < b)
    {
        return -1;
    }
    else if (a > b)
    {
        return 1;
    }

    return 0;
}

var requirements = [];
var tickets = {};

https://insightcloud.digitalglobe.com/redmine/projects/hootenany/issues.csv?utf8=%E2%9C%93&columns=all

var request = https.request({
    host:'insightcloud.digitalglobe.com',
    port:443,
    path:'/redmine/projects/hootenany/issues.xml?key=50bb4ce472d38cd3233e0db13d582ee9d834e633&query_id=32&columns=all&limit=1000',
    method: 'GET',
    rejectUnauthorized:false,
    requestCert: true,
    agent: false},
    function(res)
{
    var body = []
    res.on('error', function(e) {
        console.log('problem with request: ' + e.message);
    });
    res.on('data', function(chunk)
    {
        body.push(chunk);
    });
    res.on('end', function(){
        xml2js.parseString(body.join(''), function(err, data)
        {
            if (Number(data.issues['$'].limit) < Number(data.issues['$'].total_count))
            {
                // Hi future self. Welcome back to the code. Hopefully it has
                // been a long, long time.
                // Find your solution here:
                // http://www.redmine.org/projects/redmine/wiki/Rest_api#Collection-resources-and-pagination
                throw new Error("Dear future self, Sorry, you just ran into a limit of the JS report parsing code. You're going to have to crawl into the code and add pagination. See comments for details.");
            }

            var issues = data.issues.issue;
            for (i in issues)
            {
                if (issues[i].parent)
                {
                    var parentId = issues[i].parent[0]['$'].id;
                    if (!tickets[parentId].children)
                    {
                        tickets[parentId].children = []
                    }
                    tickets[parentId].children.push(issues[i].id[0]);
                }
                tickets[issues[i].id[0]] = issues[i];
            }
            populateDescriptions();
        });
    });
});
request.end();

function populateDescriptions() {
var body = '';
//'https://insightcloud.digitalglobe.com/redmine/projects/hootenany/issues.atom?key=9fa3feec8dcb79cc44d13ce730cecc862ed77269&per_page=1000&query_id=30', 
var request = https.request({
    host:'insightcloud.digitalglobe.com',
    port:443,
    path:'/redmine/projects/hootenany/issues.atom?key=9fa3feec8dcb79cc44d13ce730cecc862ed77269&per_page=1000&query_id=32',
    method: 'GET',
    rejectUnauthorized:false,
    requestCert: true,
    agent: false},
    function(res)
{
    var body = []
    res.on('error', function(e) {
        console.log('problem with request: ' + e.message);
    });
    res.on('data', function(chunk)
    {
        body.push(chunk);
    });
    res.on('end', function(){
        xml2js.parseString(body.join(''), function(err, data)
        {
            console.log()
            console.log("== Requirements Status");
            console.log();
            var entries = data.feed.entry;
            for (var k in entries)
            {
                req.ticket = tickets[parseTicket(entries[k].title[0])];
                req.status = parseStatus(entries[k].title[0]);
                req.updated = entries[k].updated[0];
                toAsciiDoc(req, entries[k].content[0]["_"]);
                if (req.title == undefined)
                {
                    req.title = getTitle(entries[k].title[0]);
                }
                requirements.push(req);
                req = {};
            }
            requirements.sort(compareReq); 

            for (var r in requirements)
            {
                req = requirements[r];
                console.log('[float]\n=== ' + requirements[r].title.trim() + '');
                console.log();
                var timeStr = '-'
                if (req.ticket.estimated_hours[0] > 0)
                {
                    time = Number(req.ticket.estimated_hours[0]);
                    timeMonths = (time / (153.3)).toFixed(1);
                    timeStr = time + 'hours (' + timeMonths + 'months)';
                    if (req.ticket.children)
                    {
                        timeStr = "*" + timeStr + "*";
                    }
                }
                var poc = "_Unassigned_";
                if (req.ticket.assigned_to)
                {
                    poc = req.ticket.assigned_to[0]['$'].name
                }
                console.log('|======');
                console.log('|Status | Last Updated | Estimated Time | DG Technical POC');
                console.log('|' + requirements[r].status + 
                            ' | ' + new Date(req.ticket.updated_on).toLocaleDateString() +
                            ' | ' + timeStr +
                            ' | ' + poc);
                console.log('|======');
                console.log();
                console.log(requirements[r].text);
                console.log();
            }
            printTrl();
            console.log();
            console.log("== Definitions");
            console.log();
            console.log("* For the purposes of this document 1 person month = 153.3 person hours.");
            console.log("* DG Technical POC - The person assigned to tracking the requirement.");
            console.log("* Defined - The requirement has enough information that work can begin without further definition.");
            console.log("* New/Undefined - There is not enough definition to begin work, or outstanding questions may change the direction enough that starting work would not be wise.");
            console.log("* In Progress - Work has begun on the task.");
            console.log("* Completed/Ready for Test - The task is considered complete by the owner of the task, but it is awaiting acceptance or testing.");
            console.log("* Accepted/Closed - The task has been accepted as complete by relevant party.");
            console.log();
            console.log("NOTE: On the person month: We picked a somewhat odd number of 153.3 person hours in a month. The rational is as follows. There doesn't appear to be a clear definition of the number of hours in a person year. The definition seems to range from 1800 to 2080. We chose 1840 as the person year given the following calculation _1840 hours = 52 weeks * 40 hours - 10 days holiday - 20 days vacation_. Dividing 1840 by 12 gives ~153.3. A little odd, but it works. Regardless, all estimates are in person hours so feel free to reinterpret into person months/years as you see fit.");
        });
    });
});
request.end();
}

function printTrl()
{
console.log("== DoD Software Technology Readiness Level (TRL)");
console.log();
console.log("Provided for reference (http://ieee-stc.org/proceedings/2010/pdfs/CD2694.pdf)");
console.log();
console.log("1.  Basic principles observed and reported.");
console.log("2.  Technology concept and/or application formulated Technology concept and/or application formulated.");
console.log("**   Concepts and ideas will be presented to the customer in a document or slides.");
console.log("3.  Analytical and experimental critical function and/or characteristic proof of concept");
console.log("**   Work will be executed on a DigitalGlobe designated workstation and results presented as a proof of concept.");
console.log("4.  Module and/or subsystem validation in a laboratory environment, i.e. software prototype development environment ");
console.log("**   Work will be demonstrated on a DigitalGlobe designated workstation on real world data. E.g. the specific feature will be demonstrated via the command line interface on a DG workstation.");
console.log("5.  Module and/or subsystem validation in a relevant environment ");
console.log("**   Work will be demonstrated on the ILE Network. E.g. the specific feature will be demonstrated via the command line interface on real world data.");
console.log("6.  Module and/or subsystem validation in a relevant end-to-end environment ");
console.log("**   Work will be demonstrated within the larger system on the ILE Network. E.g. the feature will be demonstrated within the iD interface and within the context of a use case.");
console.log("7.  System prototype demonstration in an operational high fidelity environment ");
console.log("**   Work will be demonstrated within a T approved environment.");
console.log("8.  Actual system completed and mission qualified through test and demonstration in an operational environment ");
console.log("**   IOC");
console.log("9.  Actual system proven through successful mission proven operational capabilities. ");
console.log("**   FOC");
console.log("");
}

