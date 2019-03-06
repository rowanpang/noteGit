function usage () {
    echo "Usage :  $0 [options] [optIssues]
        Options:
        -h          Display this message
        -d          dryRun
        -c          doClean
	-vp	    multChar test
    "
}

function main(){
    while getopts "hdcvp" opt;do
        case $opt in
            h)
                usage
                exit 0
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
	    \?)
		echo "Invalid option: -$OPTARG"
		usage
		;;
	esac
    done

    echo "$dryRun,$cleanRun,$multChar"
}

main $@
