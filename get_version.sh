#!/bin/bash

if [ -d .git ]; then
    (VER=`git describe --match=v* HEAD --tags` && echo $VER|sed 's/^v//') ||
    (echo "0.0-"`git rev-parse --abbrev-ref HEAD`"-"`git rev-list --full-history --all --abbrev-commit | head -1`);
elif [ -d .svn ]; then
    echo `svn info | grep URL: | awk '{n=split($2,arr,"/"); print arr[n];}'|sed 's/^v//'`'-rev'`svn info | grep Revision: | cut -c11-`
else
    echo '0.0-unknow';
fi
