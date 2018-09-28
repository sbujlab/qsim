#!/bin/bash

ERR_OPT=10
ERR_BAD_FILE=20
ERR_NO_ROOTFILE=21

function help {
    echo
    echo Syntax:
    echo -e "$(basename $0)\t[--convert|--plot|--z|--help]"
}

function checkfile(){
    [ ! -e "$1" ] && echo "$1 doesn't exist; please check your input." && exit $ERR_BAD_FILE
}

layer=false
convert=false
z=false
plot=false   
default=true  # default behavior
if [ $# -gt 0 ]; then
    opts=`getopt --longoptions layer,convert,z,plot,help -- "$@"`
    [ $? != 0 ] && echo "Fail to parse options." >&2 && help && exit $ERR_OPT
    while true; do
	case "$1" in
	    --layer)
		layer=true;
		default=false;
		shift ;;
	    --convert)
		convert=true; 
		default=false;
		shift ;;
	    --z)
		z=true;	
		default=false;
		shift ;;
	    --plot)
		plot=true; shift ;;
	    --help)
		help && exit 0;;
	    --)
		shift
		break;;
	    *)
		break;;
	esac
    done
fi

if [ $# -eq 0 ]; then
    echo "No root file is specified" >&2 && help && exit $ERR_NO_ROOTFILE
fi

qsim_root_file=$1;
checkfile $qsim_root_file

wd="/home/weibin/showerMax_1/analysis"

if $convert; then
    echo "Convert root file:"
    "$wd"/convert_layer $qsim_root_file
fi

if $z; then
    echo "Plot z:"
    "$wd"/z $qsim_root_file
fi

if $layer; then
    echo "Plot layers:"
    "$wd"/layer $qsim_root_file
fi

if $plot; then
    echo "Plot:"
    "$wd"/plot $qsim_root_file
elif $default; then
    echo "Nothing specified, use default option:"
    "$wd"/plot $qsim_root_file
fi
