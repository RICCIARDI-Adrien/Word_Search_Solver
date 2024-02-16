#!/bin/sh

# Check arguments
if [ $# -ne 1 ]
then
	printf "Fetch a https://mots-meles.relaxweb.fr grid and convert it to a word search file.\n"
	printf "Usage : $0 Relaxweb_Grid_URL\n"
	exit 1
fi

# Extract the grid number from the URL by using a little trick, as the separators are the sames than an UNIX file system
Grid_Number=$(basename $1)

# Download the web page
wget $1 -O /tmp/Relaxweb_${Grid_Number}.html
if [ $? -ne 0 ]
then
	printf "Error : could not download the web page $1.\n"
	exit 1
fi

# Retrieve the JavaScript line containing the words list because it also contains the grid size
Raw_Line=$(grep "wordSearchInit" /tmp/Relaxweb_${Grid_Number}.html)
if [ $? -ne 0 ]
then
	printf "Error : could not find the JavaScript line starting with \"wordSearchInit\".\n"
	exit 1
fi

# Extract the grid columns count
Grid_Size_Line=$(echo $Raw_Line | sed -e 's/wordSearchInit(\[.*\],//')
Grid_Columns_Count=$(echo $Grid_Size_Line | cut -d ',' -f 1)
Grid_Rows_Count=$(echo $Grid_Size_Line | cut -d ',' -f 2)

# Extract the words to search and convert french unicode characters to ASCII
Words_List=$(echo $Raw_Line | sed -e 's/wordSearchInit(\[.*\],\[//;s/\],[0-9].*//;s/\"//g;s/,/ /g;s/À/A/g;s/Â/A/g;s/Ä/A/g;s/Ç/C/g;s/É/E/g;s/È/E/g;s/Ê/E/g;s/Ë/E/g;s/Ô/O/g;s/Î/I/g;s/Ï/I/g;s/Û/U/g')

# Retrieve the HTML line containing the grid letters
Raw_Line=$(grep "<span id=\"char_" /tmp/Relaxweb_${Grid_Number}.html)
if [ $? -ne 0 ]
then
	printf "Error : could not find the HTML line starting with \"<span id=\"char_\".\n"
	exit 1
fi

# Extract the grid content and replace non-ASCII characters
Grid_Letters=$(echo $Raw_Line | sed -e 's/<span id="char_[0-9]*_[0-9]*" class="char">//g;s/<\/span>//g;s/<br>//g;s/<\/div>//g;s/ //g;s/À/A/g;s/Â/A/g;s/Ä/A/g;s/Ç/C/g;s/É/E/g;s/È/E/g;s/Ê/E/g;s/Ë/E/g;s/Ô/O/g;s/Î/I/g;s/Ï/I/g;s/Û/U/g')

# Clear any previous file
File_Name=Relaxweb_${Grid_Number}.txt
echo -n > $File_Name

# Create the grid content
for Row in $(seq 1 $Grid_Rows_Count)
do
	for Column in $(seq 1 $Grid_Columns_Count)
	do
		# Write the first letter to the file
		echo -n $(echo $Grid_Letters | cut -c 1) >> $File_Name
		# Remove the first letter from the string
		Temporary_String=$(echo $Grid_Letters | cut -c 2-)
		Grid_Letters=$Temporary_String
	done

	# Go to next row
	echo >> $File_Name
done

# Append separator
echo "-" >> $File_Name

# Append words list
for Word in $Words_List
do
	echo $Word >> $File_Name
done
