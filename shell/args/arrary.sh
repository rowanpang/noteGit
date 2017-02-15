function f() {
    name=$1[@]
    a=("${!name}")

    for i in "${a[@]}" ; do
        echo "$i"
    done
    echo "b: $b"

    for i in "${!a[@]}";do
	echo "member $i: ${a[i]}"
    done
}

x=("one two" "LAST"   #1-2
 "three" "four"       #3-4
 )
b='even more'

f x "$b"


macs="zero one two three four five"
index=0
for mac in $macs;do
    [ $index -eq 3 ] && echo $mac
    let index+=1
done
