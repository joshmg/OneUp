## test.sh
#!/bin/bash

#          "path/to/dir/*.ext"
test_files="unit_test/*.s"

echo "------------------------------------------------------------------"
echo "|                Running all test files...                       |"
echo "------------------------------------------------------------------"

for file in `ls $test_files`; do
	echo "Testing $file."
	echo  
	./wi11-asm $file out.o  # run the assembler
	echo
	echo ">> out.o"
	cat out.o 2>/dev/null   # print the object file
	rm out.o 2>/dev/null    # remove the object file
	echo
	echo 
	echo -n "Next? "
	read
	echo "------------------------------------------------------------------"
done

