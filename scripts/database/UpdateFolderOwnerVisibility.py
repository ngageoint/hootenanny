#!/usr/bin/env python
from subprocess import check_output as co
import argparse, os, sys, csv
from StringIO import StringIO
devnull = open(os.devnull, 'w')
# devnull = sys.stdout

parser = argparse.ArgumentParser(description='Process some integers.')
group = parser.add_mutually_exclusive_group(required=True)
group.add_argument('--folder_name', '-f', help='name of parent folder', type=str)
group.add_argument('--folder_id', '-i', help='id of parent folder', type=int)
group = parser.add_mutually_exclusive_group(required=True)
group.add_argument('--public',  action='store_const', const=True)
group.add_argument('--private', action='store_const', const=False)
parser.add_argument('user_id', help='user id of new owner', type=int)
args = parser.parse_args()

if args.folder_id is not None and args.folder_id == 0:
    print 'not valid for root'
    sys.exit(1)

if args.folder_name is not None and args.folder_name == 'root':
    print 'not valid for root'
    sys.exit(1)

sql = "copy (select * from users where id='%s') to stdout csv" % args.user_id
cmd = ['sudo', '-u', 'postgres', 'psql', '-d', 'hoot', '-c', sql]
stdout = co(cmd, stderr=devnull)

if len(stdout) == 0:
    print 'user %s not found' % args.user_id
    sys.exit(1)

if args.folder_name is not None:
    sql = "copy (select count(1) from folders where display_name = '%s') to stdout csv" % args.folder_name
    cmd = ['sudo', '-u', 'postgres', 'psql', '-d', 'hoot', '-c', sql]
    stdout = co(cmd, stderr=devnull)
    rows = stdout.split('\n')
    row = rows[0]
    count = int(row)
    if count > 1:
        print 'folder name collides, cannot continue w/ display_name'
        sys.exit(1)
else:
    sql = "copy (select count(1) from folders where id = '%s') to stdout csv" % args.folder_id
    cmd = ['sudo', '-u', 'postgres', 'psql', '-d', 'hoot', '-c', sql]
    stdout = co(cmd, stderr=devnull)
    rows = stdout.split('\n')
    row = rows[0]
    count = int(row)
    if count != 1:
        print 'folder with id %d not found' % args.folder_id
        sys.exit(1)
where = None
if args.folder_name is not None:
    where = " display_name = '%s' " % args.folder_name
else:
    where = " id = %d " % args.folder_id

sql = """copy (
    with recursive related_folders as (
         select id,parent_id,display_name,user_id,public,created_at from folders where %s
         union
         select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f
         inner join related_folders rf on (
              f.id != 0 AND
              (f.parent_id = rf.id OR f.id = rf.parent_id)
         )
    )
    select * from related_folders) to stdout csv
""" % where

cmd = ['sudo', '-u', 'postgres', 'psql', '-d', 'hoot', '-c', sql]
stdout = co(cmd, stderr=devnull)
f = StringIO(stdout)
reader = csv.reader(f, delimiter=',')

# ['128', '0',   'TestFolder',    '-1541432234', 't', '2018-11-01 10:47:02.395+00']
# ['129', '128', 'TestSubFolder', '-1541432234', 't', '2018-11-01 10:47:55.771+00']

print '#> the following folders will be affected:'
for row in reader:
    print "folder with id {:>5}, name {:>20}, visibility {:>5}, and current owner {:>20}".format(row[0], row[2], "true" if row[4] == 't' else "false", row[3])

sql = """copy (
    with recursive related_folders as (
         select id,parent_id,display_name,user_id,public,created_at from folders where %s
         union
         select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f
         inner join related_folders rf on (
              f.id != 0 AND
              (f.parent_id = rf.id OR f.id = rf.parent_id)
         )
    )

    select m.id, m.display_name, m.user_id, m.public from maps m
    left join folder_map_mappings f on (f.map_id = m.id)
    where f.folder_id in (select id from related_folders)

    ) to stdout csv
""" % where
cmd = ['sudo', '-u', 'postgres', 'psql', '-d', 'hoot', '-c', sql]
stdout = co(cmd, stderr=devnull)
f = StringIO(stdout)
reader = csv.reader(f, delimiter=',')

# ['128', '0',   'TestFolder',    '-1541432234', 't', '2018-11-01 10:47:02.395+00']
# ['129', '128', 'TestSubFolder', '-1541432234', 't', '2018-11-01 10:47:55.771+00']

print '#> the following maps will be affected:'
for row in reader:
    print "map with id {:>5}, name {:>20}, visibility {:>5}, and current owner {:>20}".format(row[0], row[1], "true" if row[3] == 't' else "false", row[2])

print ''
try:
    continu = raw_input("'y' to continue (n)$ ")
except Exception as e:
    print e
    continu = ''

if continu.strip() != 'y':
    sys.exit(0)

print '#> running update...'

sql =  """BEGIN;
    with recursive related_folders as (
         select id,parent_id,display_name,user_id,public,created_at from folders where %s
         union
         select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f
         inner join related_folders rf on (
              f.id != 0 AND
              (f.parent_id = rf.id OR f.id = rf.parent_id)
         )
    )

    update folders set user_id=%d, public=%s where id in (select id from related_folders)
""" % (where, args.user_id, args.public or args.private)

sql =  sql + """;
    with mapids as (
        with recursive related_folders as (
             select id,parent_id,display_name,user_id,public,created_at from folders where %s
             union
             select f.id,f.parent_id,f.display_name,f.user_id,f.public,f.created_at from folders f
             inner join related_folders rf on (
                  f.id != 0 AND
                  (f.parent_id = rf.id OR f.id = rf.parent_id)
             )
        )

        select m.id from maps m
        left join folder_map_mappings f on (f.map_id = m.id)
        where f.folder_id in (select id from related_folders)
    )

    update maps set user_id = %d where id in (select id from mapids); COMMIT;
""" % (where, args.user_id)
cmd = ['sudo', '-u', 'postgres', 'psql', '-a', '-d', 'hoot', '-c', sql]
stdout = co(cmd, stderr=devnull)
print stdout.strip()
