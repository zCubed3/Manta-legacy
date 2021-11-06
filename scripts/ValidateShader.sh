#!/bin/sh

echo "Attempting to validate $1"

VERT_STATE=$(glslangValidator -S frag --define-macro FRAGMENT -l $1)

if [ $? == "0" ]; then
  echo "Vert passed!"
else
  echo "Vert failed, printing log..."
  echo $VERT_STATE
fi

FRAG_STATE=$(glslangValidator -S vert --define-macro VERTEX -l $1)

if [ $? == "0" ]; then
  echo "Frag passed!"
else
  echo "Frag failed, printing log..."
  echo $FRAG_STATE
fi