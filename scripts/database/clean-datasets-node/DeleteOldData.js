var pg = require('pg');
var moment = require('moment');
var now = moment();
// config
var pgclient = new pg.Client({
  host: 'localhost',
  database: 'hoot',
  port: 5432,
  user: 'hoot',
  password: 'hoottest'
});

// disconnect
function disconnect() {
  pgclient.end(function(err) {
    console.log('client has disconnected');
  });
}

pgclient.connect(function(err){
  if (err) {
    console.error('connection error', err.stack);
  } else {
    // client has connected to pg
    console.log('connected');
    // get dataset ids, names, and lastAccessed dates from pg table
    var query = pgclient.query("select id, display_name, (a).value from ( \
                                  select *, each(tags) as a \
                                  from maps \
                                ) t;");
    var rows = [];
    query.on('row', function(row, res) {
      rows.push(row);
    });
    query.on('end', function(result){
      if(rows[0] === undefined){
        // if no results are returned, disconnect
        console.log('no data returned');
        disconnect();
      } else {
        // create new array with data id , last access date, and whether or not it's stale
        var data = rows.map(function(row){
          var stale = moment().diff(moment(row.value), 'days') > 60;
          return {
            id: row.id,
            name: row.display_name,
            accessed: row.value,
            stale: stale
          }
        });
        console.log(data);
        // delete stale datasets
        data.forEach(function(d){
          if (d.stale) {
            pgclient.query("delete from maps where id=" + d.id);
            console.log("deleted dataset " + d.name);
          }
        })
      }
      disconnect();
    })
  }
});
