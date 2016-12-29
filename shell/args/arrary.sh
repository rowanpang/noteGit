function f() {
    name=$1[@]
    a=("${!name}")

    for i in "${a[@]}" ; do
        echo "$i"
    done
    echo "b: $b"
}

x=("one two" "LAST")
b='even more'

f x "$b"
