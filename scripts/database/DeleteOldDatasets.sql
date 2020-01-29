--Deletes map datasets that have not been accessed in over 6 months
--and then removes any resulting empty folders

DO $$

DECLARE
    map_id BIGINT;
    ts TIMESTAMP := NOW() - INTERVAL '6 months';
BEGIN

FOR map_id IN
    SELECT id
    FROM maps
    WHERE (tags -> 'lastAccessed' IS NULL AND created_at < ts)
    OR TO_TIMESTAMP(tags -> 'lastAccessed', 'YYYY-MM-DD"T"HH24:MI:SS.MS"Z"') < ts
LOOP
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('current_way_nodes_' || map_id);
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('current_relation_members_' || map_id);
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('current_nodes_' || map_id);
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('current_ways_' || map_id);
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('current_relations_' || map_id);
    EXECUTE 'DROP TABLE IF EXISTS ' || quote_ident('changesets_' || map_id);
    EXECUTE 'DROP SEQUENCE IF EXISTS ' || quote_ident('current_nodes_' || map_id || '_id_seq');
    EXECUTE 'DROP SEQUENCE IF EXISTS ' || quote_ident('current_ways_' || map_id || '_id_seq');
    EXECUTE 'DROP SEQUENCE IF EXISTS ' || quote_ident('current_relations_' || map_id || '_id_seq');
    EXECUTE 'DROP SEQUENCE IF EXISTS ' || quote_ident('changesets_' || map_id || '_id_seq');
    EXECUTE 'DELETE FROM maps WHERE id = ' || map_id;
    RAISE INFO 'Deleted dataset with map id %', map_id;
END LOOP;

END $$;


--Deletes folders that are empty (no child datasets or folders)
DELETE
FROM folders f
WHERE
NOT EXISTS
    (
    SELECT  NULL
    FROM    folder_map_mappings fmm
    WHERE   f.id = fmm.folder_id
    )
AND
NOT EXISTS
    (
    SELECT  NULL
    FROM    folders f2
    WHERE   f.id = f2.parent_id
    );
