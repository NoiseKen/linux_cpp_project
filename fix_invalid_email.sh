#!/bin/sh

git filter-branch --env-filter '

am="$GIT_AUTHOR_EMAIL"
cm="$GIT_COMMITTER_EMAIL"

if [ "$GIT_COMMITTER_EMAIL" = "root@localhost.localdomain" ]
then
    cm="noiseken@gmail.com"
fi
if [ "$GIT_AUTHOR_EMAIL" = "root@localhost.localdomain" ]
then
    am="noiseken@gmail.com"
fi

export GIT_AUTHOR_EMAIL="$am"
export GIT_COMMITTER_EMAIL="$cm"
'
