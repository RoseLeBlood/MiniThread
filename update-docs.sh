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
function git_push() {
	cd "$MN_DEF_BASE_DIR/doc/$MN_DEF_MAIN_PATH"
	git commit -a -m "update to version ${MN_DEF_VERSION}"
	git push
}
function print_help() {
	echo "using:
	$0 create	- create the documentation
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
            ;;
        push)
        	cd doc/
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

