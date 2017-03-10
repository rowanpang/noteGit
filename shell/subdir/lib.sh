#this not a stand-alone script,this provides funtions.

function funcShare(){
    echo "function in lib.sh,from $1"
}

function libinit(){
    echo "libinit,will do every source $libinited"
    libinited="yes"
    varValuedbylibInit="lib init test"
}

program=$0
localdir=$(dirname $program)
[ "$1" ==  "libinit" ] && libinit
