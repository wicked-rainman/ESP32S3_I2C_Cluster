#!/bin/bash
grep -v Bcast cute.log  | grep -v Roll > dout.txt
echo "digraph d {" > dotout.dot
while IFS= read -r line
do
  echo $line | awk -F "," '{print "\""$1"\" -> \""$2"\" ;"}' >> dotout.dot
  echo $line | awk -F "," '{print "\""$1"\" [label = \""$1"\\n("$4")\\n"$5"\"];"}' >> dotout.dot
  echo $line | awk -F "," '{print "\""$2"\" [label = \""$2"\\n("$6")\\n"$7"\"];"}' >> dotout.dot
done < dout.txt
echo "}" >> dotout.dot
sfdp dotout.dot -Tpdf > test1.pdf
