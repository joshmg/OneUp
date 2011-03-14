#!/bin/bash
# organize.sh

# backup directory name
mkdir orig cpp h &> /dev/null

for file in `ls`; do
	if [ ! -d "$file" ]; then
		# get file extension
		extension=${file#*.}

		if [ $extension == "cpp" ]; then
			# move cpp and h files to appropriate directories
			cat $file | sed 's/"\([A-Za-z].*\.h\)"/"\.\.\/h\/\1"/g' > ./$extension/$file
			mv $file orig/
		fi

		if [ $extension == "h" ]; then
			cp $file h/
			mv $file orig/
		fi
	fi
done

# remove any that aren't used
rmdir orig cpp h &> /dev/null

