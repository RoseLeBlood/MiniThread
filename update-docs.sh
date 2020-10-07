#!/bin/zsh 

DOCSGIT=../../mnthread-docs/docs

cd doc
doxygen

# internal use
#rm -r $DOCSGIT/
#mkdir -p $DOCSGIT/css $DOCSGIT/js

# copy jquery and bootstrap (never move - not generated)
cp jquery-2.1.1.min.js  $DOCSGIT/js/.
cp bootstrap.min.js     $DOCSGIT/js/.
cp bootstrap.min.css    $DOCSGIT/css/.
cp robotslab.css        $DOCSGIT/css/.

# wv css and js files to directoy (generated )
mv html/*.css           $DOCSGIT/css/.
mv html/search/*.css    $DOCSGIT/css/.
mv html/*.js            $DOCSGIT/js/.
mv html/search/*.js     $DOCSGIT/js/.

# copy all other to dos 
cp --recursive html/* $DOCSGIT/.
cd $DOCSGIT/..

git add .
git commit -m "update version"
#git push

echo $ARG1
