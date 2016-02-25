#/bin/bash

IFS_BAK="$IFS"
echo "old ifs"
echo -n "$IFS_BAK" | xxd -g 1

function oneline_onearg()
{
	arg_list=$*
	echo "\$*='$*'"
	i=1
	for arg in $arg_list
	do
		echo "$i: "[$arg]""
		let "i += 1"
	done
}
#test how ifs influence the input
#input:"a&ad"
#output:"a ad"
function ifs_input()
{
	echo "new ifs: '&'"
	IFS='&'
	while [ "$my" != "q" ]
	do
		echo "-----new loop---"
		read 'input'
		echo "input is:"\"$input\"
		oneline_onearg $input
	done
}

ifs_input

echo "new ifs"
echo -n "$IFS" | xxd -g 1
IFS=$IFS_BAK

echo "restore ifs"
echo -n "$IFS" | xxd -g 1
