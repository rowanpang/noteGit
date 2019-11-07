function usage () {
    echo "Usage :  $0 [options] [optIssues]
        Options:
        -h          Display this message
        -d          dryRun
        -c          doClean
	-vp	    multChar test
	-a  n	    test [$n]
    "
    exit 0
}

function main(){
    n=3
    #while getopts ":hdca:vp" opt;do	    #first colon silent error mode
    while getopts "hdca:vp" opt;do
        case $opt in
            h)
                usage
                ;;
            d)
                dryRun="True"
                ;;
            c)
                cleanRun="True"
		;;
	    vp)
		multChar="True"
		echo "multChar args"
		;;
	    a)
		n="$OPTARG";
		;;
	    \?)
		echo "Invalid option: $OPTARG"
		usage
		;;
	    :)
                echo "--Need arguement $OPTARG"
                usage
		;;

	esac
    done

    echo "$dryRun,$cleanRun,$multChar,$n"
}

main $@
