#!/bin/bash

GREEN="\033[32m"
RED="\033[31m"
DEFAULT="\033[37m"
CYAN="\x1b[36m"
USER="Result/"
USER2="Bfs_one_path/"
VALGRIND_DIR="Valgrind/"
DIFF="Diff/"
P="./lem-in"
DIR="Maps/"

rm -dR Result 2>/dev/null
mkdir -p Result
rm -dR Valgrind 2>/dev/null
mkdir -p Valgrind

nm -u ./lem-in

if ! command -v valgrind &> /dev/null; then
    printf "${RED}valgrind n'est pas installé. Installe-le avec: sudo apt install valgrind${DEFAULT}\n"
    exit 1
fi

FILE="1.map"
FILE1="9_path_depth_4_ants.map"
FILE2="2.map"
FILE3="5.map"
FILE4="10K.map"
FILE5="100K.map"
FILE7="10_lots_of_comments.map"
FILE8="20.map"
FILE9="err_3_coordinates.map"
FILE10="err_duplicate.map"
FILE11="err_no_ants2.map"
FILE12="err_no_ants.map"
FILE13="err_too_few_ants.map"
FILE14="err_no_links.map"
FILE15="err_empty.map"
FILE16="err_no_ants3.map"
FILE17="err_no_rooms.map"
FILE18="err_letter_in_coordinate.map"
FILE19="err_link_to_unknown_room.map"
FILE20="9_path_depth_2_ants.map"
FILE21="9_path_depth_8_ants.map"
FILE22="9_path_depth_9_ants.map"
FILE23="9_path_depth_10_ants.map"
FILE24="err_no_end2.map"
FILE25="err_no_start2.map"
FILE26="err_start_end_same.map"
FILE27="err_too_many_ants.map"
FILE28="err_no_start.map"
FILE29="err_no_ants3.map"
FILE30="err_no_rooms.map"
FILE31="err_letter_in_coordinate.map"
FILE32="err_no_end.map"

run_test() {
    local file="$1"
    local input_path="$2"
    local safe_name
    safe_name=$(basename "$file" | tr '/' '_')

    printf "\n\n${GREEN}${file}${DEFAULT}\n"

    timeout 10 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
        --error-exitcode=42 \
        "$P" < "$input_path" > "${USER}${safe_name}" \
        2> "${VALGRIND_DIR}${safe_name}.valgrind.log"

    exit_code=$?

    if [ $exit_code -eq 42 ]; then
        printf "${RED}⚠️  Valgrind error/leak detected for %s (see %s%s.valgrind.log)${DEFAULT}\n" "$file" "$VALGRIND_DIR" "$safe_name"
    elif [ $exit_code -eq 124 ]; then
        printf "${RED}⏱️  Timeout for %s${DEFAULT}\n" "$file"
    elif [ $exit_code -ge 128 ]; then
        printf "${RED}💥 Crash detected for %s (signal %d)${DEFAULT}\n" "$file" "$((exit_code - 128))"
    fi

    printf "${CYAN}Output: cat ${USER}${safe_name}${DEFAULT}\n"
}

run_test "$FILE" "$DIR$FILE"
run_test "$FILE1" "$DIR$FILE1"
run_test "$FILE20" "$DIR$FILE20"
run_test "$FILE21" "$DIR$FILE21"
run_test "$FILE22" "$DIR$FILE22"
run_test "$FILE23" "$DIR$FILE23"
run_test "$FILE2" "$DIR$FILE2"
run_test "$FILE3" "$DIR$FILE3"

# run_test "$FILE4" "$DIR$FILE4"
# run_test "$FILE5" "$DIR$FILE5"

run_test "$FILE7" "$DIR$FILE7"
run_test "$FILE8" "$DIR$FILE8"
run_test "$FILE24" "$DIR$FILE24"
run_test "$FILE9" "$DIR$FILE9"
run_test "$FILE10" "$DIR$FILE10"
run_test "$FILE11" "$DIR$FILE11"
run_test "$FILE12" "$DIR$FILE12"
run_test "$FILE13" "$DIR$FILE13"
run_test "$FILE14" "$DIR$FILE14"
run_test "$FILE15" "$DIR$FILE15"
run_test "$FILE16" "$DIR$FILE16"
run_test "$FILE17" "$DIR$FILE17"
run_test "$FILE18" "$DIR$FILE18"
run_test "$FILE19" "$DIR$FILE19"
run_test "$FILE25" "$DIR$FILE25"
run_test "$FILE26" "$DIR$FILE26"
run_test "$FILE27" "$DIR$FILE27"
run_test "$FILE28" "$DIR$FILE28"
run_test "$FILE29" "$DIR$FILE29"
run_test "$FILE30" "$DIR$FILE30"
run_test "$FILE31" "$DIR$FILE31"
run_test "$FILE32" "$DIR$FILE32"

run_test "dev-urandom" "/dev/urandom"
run_test "dev-null" "/dev/null"
run_test "dev-zero" "/dev/zero"

printf "\n\n${GREEN}Nothing${DEFAULT}\n"
echo "" | timeout 10 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
    "$P" > "${USER}Nothing" 2> "${VALGRIND_DIR}Nothing.valgrind.log"
printf "${CYAN}Output: cat ${USER}Nothing${DEFAULT}\n"

printf "\n\n${GREEN}Output Difference:${DEFAULT}\n"
diff -r "$DIR$USER2" "$USER" > output_difference.txt
printf "${CYAN}Output: cat output_difference.txt${DEFAULT}\n"

printf "\n\n${GREEN}=== Résumé des leaks détectés ===${DEFAULT}\n"
grep -l "definitely lost" ${VALGRIND_DIR}*.log 2>/dev/null