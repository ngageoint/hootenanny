/* If any osm api db datasets are added to the cucumber tests, then the changeset id here needs to be incremented. */
INSERT INTO changesets(id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) VALUES (3, 1, (now() at time zone 'utc') + '1 day', 388845001, 389030999, -770551011, -770280999, (now() at time zone 'utc') + '1 day', 1);
