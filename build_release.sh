MODULE=MP4Tools-3.7
CODE_DIR=mp4joiner-git

rm -rf $CODE_DIR
git clone git://git.code.sf.net/p/mp4joiner/git $CODE_DIR || exit 1
(cd $CODE_DIR && chmod +x autogen.sh && ./autogen.sh && rm -rf autom4te.cache .git)
mv "$CODE_DIR" "$MODULE"

tar cjvf $MODULE.tar.bz2 $MODULE
rm -rf $MODULE
