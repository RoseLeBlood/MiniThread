cd doc
doxygen
rm -r ../../mnthread-docs/docs/

mkdir -p ../../mnthread-docs/docs/css ../../mnthread-docs/docs/js

cp jquery-2.1.1.min.js ../../mnthread-docs/docs/js/.
cp bootstrap.min.js ../../mnthread-docs/docs/js/.
cp bootstrap.min.css ../../mnthread-docs/docs/css/.

mv html/*.css  ../../mnthread-docs/docs/css/.
mv html/*.js  ../../mnthread-docs/docs/js/.

cp --recursive html/* ../../mnthread-docs/docs/.
cd ../../mnthread-docs/

git add .
git commit -m "update version"
#git push

