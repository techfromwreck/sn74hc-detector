#!/bin/zsh
clear
echo
echo
USER="techfromwreck"
HOST="MacMini"
RED=$'\033[41m'
BLK=$'\033[40;97m'
RST=$'\033[0m'

c1=(
"    .--------------.    "
" 1A |1           14| ${RED}VCC${RST}"
" 1B |2           13| 4B "
" 1Y |3           12| 4A "
" 2A |4           11| 4Y "
" 2B |5           10| 3B "
" 2Y |6            9| 3A "
"${BLK}GND${RST} |7            8| 3Y "
"    '--------------'    "
"      00 / 08 / 32      "
"    NAND / AND / OR     "
)

c2=(
"    .--------------.    "
" 1Y |1           14| ${RED}VCC${RST}"
" 1A |2           13| 4Y "
" 1B |3           12| 4B "
" 2Y |4           11| 4A "
" 2A |5           10| 3Y "
" 2B |6            9| 3B "
"${BLK}GND${RST} |7            8| 3A "
"    '--------------'    "
"           02           "
"          NOR           "
)

c3=(
"    .--------------.   "
" 1A |1           14| ${RED}VCC${RST}"
" 1Y |2           13| 6A "
" 2A |3           12| 6Y "
" 2Y |4           11| 5A "
" 3A |5           10| 5Y "
" 3Y |6            9| 4A "
"${BLK}GND${RST} |7            8| 4Y "
"    '--------------'   "
"           04           "
"      HEX INVERTER      "
)

for i in {1..11}; do
  echo "${c1[$i]}   ${c2[$i]}   ${c3[$i]}"
done
