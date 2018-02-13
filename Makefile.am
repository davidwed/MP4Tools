SUBDIRS = src locale
ACLOCAL_AMFLAGS=-I.
doc_DATA = AUTHORS COPYING INSTALL README ChangeLog

run: all
	$(MAKE) -C src run

apps: MP4Joiner.app MP4Splitter.app

MP4Joiner.app: Info.plist src/mp4joiner
	-mkdir MP4Joiner.app
	-mkdir MP4Joiner.app/Contents
	-mkdir MP4Joiner.app/Contents/MacOS
	-mkdir MP4Joiner.app/Contents/Resources
	-mkdir MP4Joiner.app/Contents/Resources/English.lproj
	cp Info.plist MP4Joiner.app/Contents/
	echo -n "APPL????" > MP4Joiner.app/Contents/PkgInfo
	cp src/mp4joiner MP4Joiner.app/Contents/MacOS/MP4Joiner
	cp resources/mp4joiner.icns MP4Joiner.app/Contents/Resources/MP4Joiner.icns
	-mkdir MP4Joiner.app/Contents/locale
	(cd locale && cp -r de es fr pl pt pt_BR ru ../MP4Joiner.app/Contents/locale/)
	cp /opt/local/bin/ffmpeg MP4Joiner.app/Contents/MacOS/
	strip MP4Joiner.app/Contents/MacOS/MP4Joiner
	strip MP4Joiner.app/Contents/MacOS/ffmpeg
	cp /Applications/Osmo4.app/Contents/MacOS/MP4Box MP4Joiner.app/Contents/MacOS/
	-mkdir MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libcrypto.1.0.0.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libgpac.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libjpeg.9.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libmozjs185.1.0.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libnspr4.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libplc4.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libplds4.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libpng16.16.dylib MP4Joiner.app/Contents/MacOS/lib/
	cp /Applications/Osmo4.app/Contents/MacOS/lib/libssl.1.0.0.dylib MP4Joiner.app/Contents/MacOS/lib/

MP4Splitter.app: Info.plist src/mp4splitter
	-mkdir MP4Splitter.app
	-mkdir MP4Splitter.app/Contents
	-mkdir MP4Splitter.app/Contents/MacOS
	-mkdir MP4Splitter.app/Contents/Resources
	-mkdir MP4Splitter.app/Contents/Resources/English.lproj
	cp InfoSplitter.plist MP4Splitter.app/Contents/Info.plist
	echo -n "APPL????" > MP4Splitter.app/Contents/PkgInfo
	cp src/mp4splitter MP4Splitter.app/Contents/MacOS/MP4Splitter
	cp resources/mp4splitter.icns MP4Splitter.app/Contents/Resources/MP4Splitter.icns
	-mkdir MP4Splitter.app/Contents/locale
	(cd locale && cp -r de es fr pl pt pt_BR ru ../MP4Splitter.app/Contents/locale/)
	cp /opt/local/bin/ffmpeg MP4Splitter.app/Contents/MacOS/
	strip MP4Splitter.app/Contents/MacOS/MP4Splitter
	strip MP4Splitter.app/Contents/MacOS/ffmpeg

libs: MP4Joiner.libs MP4Splitter.libs

%.libs:
	dylibbundler -od -b -x ./$*.app/Contents/MacOS/$* -d ./$*.app/Contents/libs/
	dylibbundler -of -b -x ./$*.app/Contents/MacOS/ffmpeg -d ./$*.app/Contents/libs/
	(cd $*.app/Contents/libs/ && rm -f libwx_*.0.2.0.dylib &&\
		ln -s libwx_baseu-3.0.dylib libwx_baseu-3.0.0.2.0.dylib &&\
		ln -s libwx_baseu_net-3.0.dylib libwx_baseu_net-3.0.0.2.0.dylib &&\
		ln -s libwx_baseu_xml-3.0.dylib libwx_baseu_xml-3.0.0.2.0.dylib &&\
		ln -s libwx_osx_cocoau_adv-3.0.dylib libwx_osx_cocoau_adv-3.0.0.2.0.dylib &&\
		ln -s libwx_osx_cocoau_core-3.0.dylib libwx_osx_cocoau_core-3.0.0.2.0.dylib &&\
		ln -s libwx_osx_cocoau_html-3.0.dylib libwx_osx_cocoau_html-3.0.0.2.0.dylib &&\
		ln -s libwx_osx_cocoau_qa-3.0.dylib libwx_osx_cocoau_qa-3.0.0.2.0.dylib &&\
		ln -s libwx_osx_cocoau_xrc-3.0.dylib libwx_osx_cocoau_xrc-3.0.0.2.0.dylib)

dmg:
	rm -f MP4Tools.dmg MP4Tools-@VERSION@-MacOSX.dmg
	hdiutil eject /dev/disk1 || echo
	hdiutil create -megabytes 150 -layout NONE MP4Tools.dmg
	hdid -nomount MP4Tools.dmg
	newfs_hfs -v MP4Tools /dev/disk1
	hdiutil eject /dev/disk1
	hdid MP4Tools.dmg
	rsync -rlv --exclude "CVS" --exclude "Makefile*" --exclude ".*" MP4Joiner.app /Volumes/MP4Tools/
	rsync -rlv --exclude "CVS" --exclude "Makefile*" --exclude ".*" MP4Splitter.app /Volumes/MP4Tools/
	cp ChangeLog /Volumes/MP4Tools/
	ln -s /Applications/ /Volumes/MP4Tools/Applications
	hdiutil detach /Volumes/MP4Tools/
	hdiutil convert MP4Tools.dmg -format UDZO -imagekey zlib-level=9 -o MP4Tools-@VERSION@-MacOSX.dmg
	rm -f MP4Tools.dmg
	
