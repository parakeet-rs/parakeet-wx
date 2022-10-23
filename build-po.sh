#!/bin/sh

set -ex

find locale -name '*.po' -type f -print0 |
  while read -r -d '' po_file; do
    mo_file="$(echo "$po_file" | sed 's/po$/mo/')"
    msgfmt -o "$mo_file" "$po_file"
  done
