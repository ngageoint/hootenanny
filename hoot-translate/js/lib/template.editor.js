// Data
var d_jobs = [
    {
        title: 'Motoways and municipal roads',
        status: 'loading'
    },
    {
        title: 'Building sites and proposed zoning',
        status: 'up',
        time: 'March 26th 2014'
    },
    {
        title: 'Landuse regulations',
        status: 'check',
        time: 'Exported on March 26th 2014'
    },
    {
        title: 'Street POI comparisons',
        status: 'check',
        time: 'Exported on March 25th 2014'
    }
];

d3.select('body').on('keydown', function(e) {
    if (d3.event.keyCode === 27) window.location.hash = '';
});

// Application Heading
var header = d3.select('body')
    .append('div')
    .attr('id', 'header')
    .classed('contain pad2x dark fill-dark', true);

    header.append('a')
        .attr('href', '/')
        .classed('inline dark strong logo pad2x keyline-right keyline-left', true)
        .html('Hootenanny <span>|</span> Innovision');

    header
        .append('nav')
        .classed('pin-right', true)
        .append('a')
        .attr('href', '#jobs')
        .classed('pad2 block keyline-left icon dark strong small sprocket', true)
        .text('Jobs');

var app = d3.select('body')
    .append('div')
    .attr('id', 'app')
    .classed('col12 pin-left', true);

var sidebar = app
    .append('div')
    .classed('col3 pad2 fill-white pin-left sidebar', true);

var map = app
    .append('div')
    .classed('col9 pin-left fill-darken margin3', true);


// Draw tools
    map.append('div')
        .classed('col12 pin-top fill-darken row1', true);

// Map controls
// map.append('div')
    // .classed('col12 pin-bottom fill-darken row1', true);

// Jobs modal
var jobs = d3.select('body').append('div')
    .attr('id', 'jobs')
    .classed('modal fill-white round col6', true);

jobs.append('div')
    .classed('pad2 fill-light round-top keyline-bottom', true)
    .append('h2').text('Jobs')
    .append('a').attr('href', '#').classed('fr strong icon x quiet', true);

jobs.append('nav')
    .classed('row6 overflow', true)
    .selectAll('div')
    .data(d_jobs)
        .enter()
        .append('div')
        .classed('col12 keyline-bottom small contain', true)
        .html(function(job) {
            var status = (job.status === 'loading') ? 'Processing' : job.time;
            var icon = (job.status === 'loading') ? 'up' : job.status;

            return '<a href="#" class="pad2x pad1y block">' +
                    job.title +
                    '<span class="quiet status fr">' +
                        status +
                    '</span>' +
                '</a>' +
                '<div class="pin-right">' +
                    '<a href="#" class="block fill-white fl pad1 keyline-left action icon js-import ' + icon + '"></a>' +
                    '<a href="#" class="block fill-white fl pad1 keyline-left action icon trash js-delete"></a>' +
                '</div>';
        });

jobs
    .append('div')
    .classed('keyline-top pad2 center round-bottom clearfix col12 fill-light', true)
        .append('a')
            .text('New job')
            .classed('center big dark col4 margin4 icon strong plus round button', true)
            .attr('href', '../import/index.html');

d3.select('body').append('div')
    .classed('mask', true);


jobs.selectAll('.js-delete').on('click', function() {
    d3.event.stopPropagation();
    d3.event.preventDefault();
});

jobs.selectAll('.js-import').on('click', function() {
    d3.event.stopPropagation();
    d3.event.preventDefault();

    var el = d3.select(this);
    el.classed('up', false).classed('loading', true);

    setTimeout(function() {
        el.classed('loading', false).classed('check', true);
        // Fire an update event to any listeners
    }, 5000);
});
