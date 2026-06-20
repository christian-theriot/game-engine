.PHONY: all run setup convert

all:
	@make -C build

setup:
	@rm -rf build
	@cmake -B build

run:
	@./build/src/game-engine

convert:
	@echo ffmpeg -i ${file}.mp3 -acodec pcm_s16le -ar 44100 ${file}.wav