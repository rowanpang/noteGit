#!/bin/bash
function prepare(){
    [ -f ./a.pdf ] && return		#consider has prepared;
    len=50
    for c in {a..e};do
	file="$c.pdf"
	echo "gen $file"
	charPrint=$(eval "printf "%0.s$c" {1..$len}")
	echo $charPrint | enscript --quiet --no-header --output=- | ps2pdf - > $file
	while ! [ -f $file ];do sleep 1;done
	[ -d merge ] || mkdir -p merge
	mv $file ./merge
    done
    cp ./merge/a.pdf .

    if ! [ -f logo-inspur.pdf ];then
	file="$HOME/Desktop/logo-inspur.pdf"
	lpr logo-inspur.jpg
	while ! [ -f $file ];do sleep 1;done
	mv $file .
    fi

    enscript --quiet --no-header --output=- bashrc | ps2pdf - bashrc.pdf
    ./cpdf -add-text "PANGWZ Page:%Page %filename %Y%m%d-%T" -topleft 15 -color "red"	\
	   -font Courier -font-size 14 -outline -diagonal bashrc.pdf -o bashrc-outline-text.pdf

}

function unprepare(){
    rm -rf merge
    rm *.pdf
}

function doTest(){
    [ -f ./a.pdf ] || prepare

    ./cpdf ./a.pdf -o a.out.pdf
    ./cpdf -merge -idir ./merge/ -o merge.out.pdf
    ./cpdf ./merge.out.pdf 2,4 -o p24.out.pdf		    #out page 2 and page 4
    ./cpdf ./merge.out.pdf 2-4 -o p2-4.out.pdf		    #out page [2,4]
    ./cpdf -encrypt AES256 "123" "456" -no-print -no-copy a.pdf -o a.AES256.pdf

    ./cpdf -stamp-on logo-inspur.pdf merge.out.pdf -o merge-logo.pdf

}

function main(){
    while [ $# -gt 0 ];do
	case $1 in
	    clean)
		unprepare
		exit
		;;
	esac
    done

    doTest
}

#---------------main------------------------
main "$@"
