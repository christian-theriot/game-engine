.PHONY: all run setup convert compile-wasm

all:
	@make -C build

setup:
	@rm -rf build
	@cmake -B build

run:
	@./build/src/game-engine

convert:
	@echo ffmpeg -i ${file}.mp3 -acodec pcm_s16le -ar 44100 ${file}.wav

compile-wasm:
	@asc assets/wasm/${file}.ts -o assets/wasm/${file}.wasm --exportRuntime --optimize

objdump-wasm:
	@wasm-objdump -x assets/wasm/${file}.wasm | grep -A 20 "Import"