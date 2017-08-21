#!/bin/bash

Red='\e[0;31m';
Green='\e[1;32m';
Gre='\e[0;32m'
Yel='\e[0;33m';
Res='\e[0m';
rm my_outputs/out*

#: <<'END'
name="tests/t_basic"
tname="outputs/out_basic"
cname="my_outputs/out_basic"
echo "################################"
echo "###  Testuji zakladni testy  ###"
echo "################################"
for i in 1 2 3 4 5 6 7 8 9 10 
do
	./ifj13 ${name}$i".ifj" < tests/stdin_file > ${cname}$i 2>tmp2
	myreturn=$?;
	valgrind --log-file="val_file" ./ifj13 ${name}$i".ifj" < tests/stdin_file > /dev/null 2> /dev/null
	if diff -u ${tname}$i ${cname}$i >tmp; then
		echo -n -e "Basic $i: ${Green}OK\t\t"
	else
		echo -n -e "Basic $i: ${Red}False\t"
		if [ "$1" != "" ]; then
			cat tmp
			echo "%%%%%"
			cat tmp2
		fi
	fi
	echo -e -n "${Res}"
	echo -e "- return :${Yel}$myreturn"
	python valgrind_test.py < val_file > val_out
	echo -e -n "${Res}"
	echo "_ _ _ _ _ _ _ _ _"

	ok_all=`sed -n '1p' val_out`
	echo -n "-> "
	if [ $ok_all == "1" ]; then
		echo -e -n "${Gre}"
	else
		echo -e -n "${Red}"
	fi
	sed -n '3p' val_out 

	ok_err=`sed -n '2p' val_out`
	echo -e -n "${Res}"
	echo -n "-> "
	if [ $ok_err == "1" ]; then
		echo -e -n "${Gre}"
	else
		echo -e -n "${Red}"
	fi
	sed -n '4p' val_out 
	
	echo -e -n "${Res}"
	echo "================================"
done

name="tests/t_prog"
tname="outputs/out_prog"
cname="my_outputs/out_prog"
echo ""
echo ""
echo "#################################"
echo "###  Testuji pokrocile testy  ###"
echo "#################################"
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
do
	
	./ifj13 ${name}$i".ifj" < tests/stdin_file > ${cname}$i 2>tmp2
	myreturn=$?;
	valgrind --log-file="val_file" ./ifj13 ${name}$i".ifj" < tests/stdin_file > /dev/null 2> /dev/null
	if diff -u ${tname}$i ${cname}$i >tmp; then
		echo -e -n "Prog $i: ${Green}OK\t\t"
	else
		echo -e -n "Prog $i: ${Red}False\t\t"
		if [ "$1" != "" ]; then
			cat tmp
			echo "%%%%%"
			cat tmp2
		fi
	fi
	echo -e -n "${Res}"
	echo -e "- return :${Yel}$myreturn"
	python valgrind_test.py < val_file > val_out
	echo -e -n "${Res}"
	echo "_ _ _ _ _ _ _ _ _"

	ok_all=`sed -n '1p' val_out`
	echo -n "-> "
	if [ $ok_all == "1" ]; then
		echo -e -n "${Gre}"
	else
		echo -e -n "${Red}"
	fi
	sed -n '3p' val_out 

	ok_err=`sed -n '2p' val_out`
	echo -e -n "${Res}"
	echo -n "-> "
	if [ $ok_err == "1" ]; then
		echo -e -n "${Gre}"
	else
		echo -e -n "${Red}"
	fi
	sed -n '4p' val_out 
	
	echo -e -n "${Res}"
	echo "================================"
done

./ifj13 ${name}"20.ifj" < tests/prog3_file > ${cname}"20" 2>tmp2
myreturn=$?;
valgrind --log-file="val_file" ./ifj13 ${name}$i".ifj" < tests/prog3_file > /dev/null 2> /dev/null
if diff -u ${tname}"20" ${cname}"20" >tmp; then
	echo -e -n "Prog 20: ${Green}OK\t\t"
else
	echo -e -n "Prog 20: ${Red}False\t"
	if [ "$1" != "" ]; then
		cat tmp
		echo "%%%%%"	
		cat tmp2
	fi	
fi
echo -e -n "${Res}"
echo -e "- return :${Yel}$myreturn"
python valgrind_test.py < val_file > val_out
echo -e -n "${Res}"
echo "_ _ _ _ _ _ _ _ _"

ok_all=`sed -n '1p' val_out`
echo -n "-> "
if [ $ok_all == "1" ]; then
	echo -e -n "${Green}"
else
	echo -e -n "${Red}"
fi
sed -n '3p' val_out 

ok_err=`sed -n '2p' val_out`
echo -e -n "${Res}"
echo -n "-> "
if [ $ok_err == "1" ]; then
	echo -e -n "${Green}"
else
	echo -e -n "${Red}"
fi
sed -n '4p' val_out 

echo -e -n "${Res}"
echo "================================"

#END

name="tests/t_extreme"
tname="outputs/out_extreme"
cname="my_outputs/out_extreme"

echo ""
echo ""
echo "################################"
echo "###  Testuji extremni testy  ###"
echo "################################"

for i in 1
do
	./ifj13 ${name}$i".ifj" < tests/stdin_file > ${cname}$i 2>tmp2
	myreturn=$?;
	valgrind --log-file="val_file" ./ifj13 ${name}$i".ifj" < tests/stdin_file > /dev/null 2> /dev/null
	if diff -u ${tname}$i ${cname}$i >tmp; then
		echo -e -n "Prog $i: ${Green}OK\t\t"
	else
		echo -e -n "Prog $i: ${Red}False\t\t"
		if [ "$1" != "" ]; then
			cat tmp
			echo "%%%%%"
			cat tmp2
		fi
	fi
echo -e -n "${Res}"
echo -e "- return :${Yel}$myreturn"
python valgrind_test.py < val_file > val_out
echo -e -n "${Res}"
echo "_ _ _ _ _ _ _ _ _"

ok_all=`sed -n '1p' val_out`
echo -n "-> "
if [ $ok_all == "1" ]; then
	echo -e -n "${Gre}"
else
	echo -e -n "${Red}"
fi
sed -n '3p' val_out 

ok_err=`sed -n '2p' val_out`
echo -e -n "${Res}"
echo -n "-> "
if [ $ok_err == "1" ]; then
	echo -e -n "${Gre}"
else
	echo -e -n "${Red}"
fi
sed -n '4p' val_out 

echo -e -n "${Res}"
echo "================================"
done



rm tmp tmp2
rm val_file
rm val_out
