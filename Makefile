gl:
	ruby tools/gl.rb

xcodebuild:
	xcodebuild -arch arm64 -target fungl -target fungl-test

.PHONY: gl xcodebuild
