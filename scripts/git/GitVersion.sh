#!/bin/bash

# Provides a slightly more pleasant version number than the typical hash value.
#
# Optionally takes a list of files. If provided, then the latest commit of
# any of the files is used for calculating the version number.
#
# If there is no version information then UNKNOWN is returned.

set -e

DEF_VER=UNKNOWN-VERSION

LF='
'

# Is this a git repo? Do we have git?
if command -v git &> /dev/null; then
    if git status &> /dev/null; then
        HAS_GIT=true
    fi
fi

REVISION=`git log -n 1 --pretty=%h $*`

# First see if there is a version file (included in release tarballs),
# then try git-describe, then default.
if test "$HAS_GIT" = "true" &&
        VN=$(git describe --match "v[0-9]*" --abbrev=7 --tags $REVISION 2>/dev/null) &&
        case "$VN" in
        *$LF*) (exit 1) ;;
        v[0-9]*)
                git update-index -q --refresh
                test -z "$(git diff-index --name-only HEAD --)" ||
                VN="$VN-dirty" ;;
        esac
then
        #VN=$(echo "$VN" | sed -e 's/-/./g');
        PASS=1
else
        VN="$DEF_VER"
fi

# VN is the full version number. E.g. 0.1.0 if it is a tag. 0.1.0-2-deadbeef 
# if it is not a tagged version
VN=$(expr "$VN" : v*'\(.*\)')

echo $VN
