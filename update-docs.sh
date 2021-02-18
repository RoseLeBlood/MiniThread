#!/bin/zsh 
VERSION="bugfix"
DOCSGIT=docs-git/docs

POSITIONAL=()
while [[ $# -gt 0 ]] do

case $1 in
    -v|--version)
    VERSION="$2"
    shift # past argument
    shift # past value
    ;;
    -d|--docsgit)
    DOCSGIT="$2"
    shift # past argument
    shift # past value
    ;;
esac
done

echo "VERSION         = ${VERSION}"
echo "DOCSGIT         = ${DOCSGIT}"


cd doc
doxygen

# internal use
rm -r $DOCSGIT/
mkdir -p $DOCSGIT/css $DOCSGIT/js



# wv css and js files to directoy (generated )
mv html/*.css           $DOCSGIT/css/.
mv html/*.js            $DOCSGIT/js/.

# copy all other to dos 
cp --recursive html/* $DOCSGIT/.


# copy jquery and bootstrap (never move - not generated)
cp jquery-3.2.1.slim.min.js  $DOCSGIT/js/.
cp bootstrap.min.js     $DOCSGIT/js/.
cp popper.min.js        $DOCSGIT/js/.

cp bootstrap.min.css    $DOCSGIT/css/.
cp bootstrap-doxy.css   $DOCSGIT/css/.

rm -r html/

cd $DOCSGIT/..
git commit -a -m "update to version ${VERSION}"
git push

echo $ARG1
