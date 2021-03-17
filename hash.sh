#/bin/sh
aaa=`echo -n $1 | md5sum`; echo -n ${aaa::-2} | shasum -a 256
