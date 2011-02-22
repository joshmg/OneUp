## test.sh
#!/bin/bash

# "path/to/dir/*.ext"
test_dir="unit_test/"
ext="*.s"

function printo() {
	echo
    echo ">>> out.o"
    cat out.o 2>/dev/null   # print the object file
}

echo "------------------------------------------------------------------"
echo "|           Running all test files in $test_dir"
echo "------------------------------------------------------------------"

for file in `ls $test_dir$ext`; do
	echo ">>> Testing $file."
	echo  
	# run the assembler and, if successful, print object file
	./wi11-asm $file out.o && printo
	echo
	# don't print anything input by the user
	stty -echo
	# restore console echoing if interupted.
	trap "stty echo; echo; exit 1" SIGINT SIGTERM
	# wait for a key-press
	read -n1 -p "(Press any key to continue)"
	# restore console echoing
	stty echo
	echo
	echo "------------------------------------------------------------------"
done

rm out.o 2>/dev/null    # remove the object file
