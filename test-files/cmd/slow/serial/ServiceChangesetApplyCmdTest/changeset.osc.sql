INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES (1, 1, now(), now());
/* create node 1 */
INSERT INTO nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 388925209, -770386440, 1, true, now(), 3221225472, 1);
INSERT INTO current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) VALUES (1, 388925209, -770386440, 1, true, now(), 3221225472, 1);
UPDATE changesets SET min_lat=388925209, max_lat=388925209, min_lon=-770386440, max_lon=-770386440 WHERE id=1;


