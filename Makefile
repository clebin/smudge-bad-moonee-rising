ASSETMAKER_PATH=../Tools/SpectrumAssetMaker/SpectrumAssetMaker.php

# compile the app

compile:
	zcc +zx -v \
		-startup=31 \
		-DWFRAMES=3 \
		-clib=sdcc_iy \
		-SO3 \
		--max-allocs-per-node200000 \
		@zproject.lst \
		-pragma-include:zpragma.inc \
		-o o/smudge \
		-m \
		-create-app

loader.tap:
	tools/bas2tap -a=10 loader.bas o/loader.tap

smudge.tap:
	z88dk-appmake +zx -b raw-assets/smudge.scr --org 16384 --noloader --blockname screen -o o/screen.tap
	z88dk-appmake +zx -b o/smudge_CODE.bin --org 24015 --noloader --blockname code -o o/code.tap
	cat o/loader.tap o/screen.tap o/code.tap > o/Smudge-Bad-Moonee-Rising.tap

tape: loader.tap smudge.tap

build: compile tape

generate-assets:
	php $(ASSETMAKER_PATH) --config=config-assets.json

run:
	open o/Smudge-Bad-Moonee-Rising.tap

play: build run

all: generate-assets build run

clean:
	-rm assets/sprites/*
	-rm assets/objects/*
	-rm assets/interface/*
	-rm assets/screens/*
	-rm o/*
