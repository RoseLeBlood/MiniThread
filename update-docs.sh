cd doc
cp --recursive html/* ../../mnthread-docs/docs/.
cd ../../mnthread-docs/

git add .
git commit -m "update version"
git push

