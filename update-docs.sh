#!/bin/sh
#set -euxo pipefail

MN_DEF_MAIN_PATH="docs-git/docs"
MN_DEF_VERSION=0
MN_DEF_BASE_DIR="$( cd "$( dirname "$0" )" && pwd )"

function parse_json() {
	JSONDATA=$(cat library.json)
	MN_DEF_VERSION=$(echo "$JSONDATA"|grep -w \"version\" |tail -1 | cut -d\" -f4)
}
function create_doxy() {
	doxygen
}
function copy_move_3third_data() {
	_MN_INSTALL_DIR="$MN_DEF_BASE_DIR/doc/$MN_DEF_MAIN_PATH"
	_MN_INSTALL_DIR_CSS="$_MN_INSTALL_DIR/css/"
	_MN_INSTALL_DIR_JS="$_MN_INSTALL_DIR/js/"

	mkdir -p $_MN_INSTALL_DIR_JS $_MN_INSTALL_DIR_CSS

	# copy all other to dos
	install -m 666 -S off  html/* $_MN_INSTALL_DIR

	# copy jquery and bootstrap (never move - not generated)
	install -m 666 -S off jquery-3.2.1.slim.min.js  $_MN_INSTALL_DIR_JS
	install -m 666 -S off bootstrap.min.js     		$_MN_INSTALL_DIR_JS
	install -m 666 -S off popper.min.js        		$_MN_INSTALL_DIR_JS

	install -m 666 -S off bootstrap.min.css    		$_MN_INSTALL_DIR_CSS
	install -m 666 -S off bootstrap-doxy.css   		$_MN_INSTALL_DIR_CSS

	# wv css and js files to directoy (generated )
	mv html/*.css           $_MN_INSTALL_DIR_CSS
	mv html/*.js            $_MN_INSTALL_DIR_JS
}
function git_push() {
	git commit -a -m "update to version ${MN_DEF_VERSION}"
}
function print_help() {
	echo "using:
	$0 create	- create the documentation and call copy
	$0 copy		- copy the docu and all using files to the docs folder - call after create
	$0 push		- push all to github repo"
}

function main() {

	if [[ $# == 0 ]]; then
        print_help
        return 0
    fi

    parse_json

	case $1 in
        create)
            shift
            cd doc
            create_doxy "$@"
            copy_move_3third_data "$@"
            ;;
		copy)
			shift
			cd doc/
			copy_move_3third_data "$@"
			;;
        push)
            shift
            git_push "$@"
            ;;
        *)
            print_help
            return 1
            ;;
    esac
}

main "$@"





#

#cd $DOCSGIT/..
#
#git push

