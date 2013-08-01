echo "enter the name of plugin:"
read a
mkdir $a
path=$(pwd)
cp -r sample/* $path/$a
cd $a
sed -i "s/sample/$a/g" * $path/$a/*

mv sample.cpp $a.cpp
mv sample.h $a.h
mv sample.json $a.json
mv sample.vcxproj $a.vcxproj 
mv sample.pro $a.pro
exit

