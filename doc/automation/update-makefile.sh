#!/bin/bash
cc=QEagleScaler
src=libs-original
out=Makefile
dir2x=libs-2x
dir4x=libs-4x
dir10x=libs-10x
nolbr=empty.lbr
wd=$PWD
cd $src
dirs=`find . -type d`
files=`find . -type f -name '*.lbr'`
cd $wd
xfiles=
for f in $files; do {
	if [[ "_" == "_"`echo -n $f | grep 'non-xml'` ]];
	then
		xfiles="$xfiles $f"
	fi;
}; done;
#./update-libs-x.sh $src 2x 0.5

echo '# This is QEagleScaler automation makefile' > $out
echo 'WRK_DIR = $(shell pwd)' >> $out
echo 'SRC_DIR = $(WRK_DIR)/'$src >> $out
echo >> $out
echo -n 'OUT_2X = ' >> $out
for f in $xfiles; do {
	o=$dir2x/${f/.lbr/.2x.lbr}
	echo -n "$o " >> $out
}; done;
#echo -e "$dir2x/$nolbr" >> $out
echo >> $out
echo >> $out
echo -n 'OUT_4X = ' >> $out
for f in $xfiles; do {
	o=$dir4x/${f/.lbr/.4x.lbr}
	echo -n "$o " >> $out
}; done;
#echo -e "$dir4x/$nolbr" >> $out
echo >> $out
echo >> $out
echo -n 'OUT_10X = ' >> $out
for f in $xfiles; do {
	o=$dir10x/${f/.lbr/.10x.lbr}
	echo -n "$o " >> $out
}; done;
#echo -e "$dir10x/$nolbr" >> $out
echo >> $out
echo >> $out
echo 'all: scale2x scale4x scale10x' >> $out
echo >> $out
echo 'scale2x: dirs2x $(OUT_2X)' >> $out
echo >> $out
echo 'scale4x: dirs4x $(OUT_4X)' >> $out
echo >> $out
echo 'scale10x: dirs10x $(OUT_10X)' >> $out
echo >> $out
echo 'dirs2x:' >> $out
for d in $dirs; do {
	o=$dir2x/$d
	echo -e "\ttest -d $o || mkdir -p $o" >> $out
}; done;
echo >> $out
echo 'dirs4x:' >> $out
for d in $dirs; do {
	o=$dir4x/$d
	echo -e "\ttest -d $o || mkdir -p $o" >> $out
}; done;
echo >> $out
echo 'dirs10x:' >> $out
for d in $dirs; do {
	o=$dir10x/$d
	echo -e "\ttest -d $o || mkdir -p $o" >> $out
}; done;
echo >> $out
for f in $xfiles; do {
	o=$dir2x/${f/.lbr/.2x.lbr}
	echo -e "$o: $src/$f" >> $out
	echo -e "\t$cc scale $src/$f $o 0.5" >> $out
	echo >> $out
}; done;
echo >> $out
for f in $xfiles; do {
	o=$dir4x/${f/.lbr/.4x.lbr}
	echo -e "$o: $src/$f" >> $out
	echo -e "\t$cc scale $src/$f $o 0.25" >> $out
	echo >> $out
}; done;
echo >> $out
for f in $xfiles; do {
	o=$dir10x/${f/.lbr/.10x.lbr}
	echo -e "$o: $src/$f" >> $out
	echo -e "\t$cc scale $src/$f $o 0.1" >> $out
	echo >> $out
}; done;
echo >> $out
echo '.PHONY: dirs2x dirs4x dirs10x' >>  $out

