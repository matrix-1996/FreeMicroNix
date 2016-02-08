#!/bin/sh
if echo "$1" | grep -Eq 'i[[:digit:]]86-'; then
  echo i686
else
  echo "$1" | grep -Eo '^[[:alnum:]_]*'
fi
