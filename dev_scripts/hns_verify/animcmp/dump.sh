# dump.sh <repo> <flat.s> <out-prefix>: assemble with the repo's pipeline and dump per-section bytes and relocations
cd "$1"
tools/preproc/preproc -s "$2" charmap.txt | arm-none-eabi-cpp -iquote include -Wno-trigraphs -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -std=gnu17 -I include - | tools/preproc/preproc -ie "$2" charmap.txt | arm-none-eabi-as -mcpu=arm7tdmi -march=armv4t -meabi=5 --defsym MODERN=1 --defsym POKEMON_HNS=1 -o "$3.o" || exit 1
arm-none-eabi-objdump -s "$3.o" > "$3.s.txt"
arm-none-eabi-objdump -r "$3.o" > "$3.r.txt"
