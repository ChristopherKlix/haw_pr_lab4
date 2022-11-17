find . -type f -name "*.[ch]" -print0 | parallel -q0 -j0 --no-notice  wc -l {} \
| awk '{ sum+=$1 }END{ print "TOTAL LINES COUNTED: "sum }'