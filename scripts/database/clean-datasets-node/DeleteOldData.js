var pg = require('pg');
var moment = require('moment');
//config
var pgclient = new pg.Client({
  host: 'localhost',
  database: 'hoot',
  port: 5432,
  user: 'hoot',
  password: 'hoottest'
});

// disconnect
function disconnect() {
pgclient.end(function(err){
  console.log('disconnected');
  if (err) {
    console.log('error during disconnection', err.stack)
  }
});
};

pgclient.connect(function(err){
  if (err) {
    console.error('connection error', err.stack);
  } else {
    // client has connected to pg
    console.log('connected');
    // get dataset ids, names, and lastAccessed dates from pg table
    var getStaleData = pgclient.query("select id, display_name, (a).value from ( \
                                  select *, each(tags) as a \
                                  from maps \
                                ) t;");
    var rows = [];
    getStaleData.on('row', function(row, result) {
      rows.push(row);
    });
    getStaleData.on('end', function(result){
      if(rows[0] === undefined){
        // if no results are returned, disconnect
        console.log('no data returned');
      } else {
        // create new array with data id , last access date, and whether or not it's stale
        var data = rows.map(function(row){
          var stale = moment().diff(moment(row.value), 'seconds') > 1;
          return {
            id: row.id,
            name: row.display_name,
            accessed: row.value,
            stale: stale
          }
        });
        // delete stale datasets
        data.forEach(function(d){
          if (d.stale) {
            pgclient.query("delete from maps where id=" + d.id);
            console.log("deleted dataset " + d.name);
          }
        });
      }
    });

    // get folders that don't have any datasets in them
    var getEmptyFolders = pgclient.query("select id, display_name from folders \
                                          where id not in \
                                          (select folder_id from folder_map_mappings)");
    var emptyFolders = [];
    getEmptyFolders.on('row', function(row, result){
      emptyFolders.push(row);
    });
    getEmptyFolders.on('end', function(result){
      if(emptyFolders[0] === undefined) {
        console.log('no empty folders');
        disconnect();
      } else {
        // delete empty folders
        emptyFolders.forEach(function(f){
          pgclient.query("delete from folders where id="+f.id);
          console.log("deleted folder " + f.display_name);
          disconnect();
        })
      }
    });
  }
});
