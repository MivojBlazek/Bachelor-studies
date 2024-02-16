#!/usr/bin/env bash

jexamxml_merlin="/pub/courses/ipp/jexamxml"

# Change this variable if you have local installation of JExamXML 
# with custom path different than merlin or current dir (".")
jexamxml="$jexamxml_merlin"

# Store old working directory
oldpwd="$(pwd)"
# Change working directory to the location of this script
cd "$(dirname "$0")"

OK() {
	printf " \e[32mOK\e[0m\n"
}

ERROR() {
	printf " \e[31mError\e[0m: $1\n"
	exit 1
}

if [ -z "$BASH" ]; then
	ERROR "This script must be run with bash"
fi

if [ ! -f parse.py ]; then
	ERROR "parse.py is missing; copy it here (or make a symlink) and try again"
fi

if [ -f jexamxml.jar ]; then
	jexamxml="."
fi

if [ ! -f "$jexamxml"/jexamxml.jar ]; then
	ERROR "jexamxml.jar is missing; are you running on merlin? \n
(alternatively, you can install Java and download the .jar and options file
from merlin on this path: "$jexamxml_merlin"/jexamxml.jar)"
fi

if [ ! -f "$jexamxml"/options ]; then
	ERROR "jexamxml options file is missing; are you running on merlin? \n
(alternatively, you can download the file from merlin on this path:
"$jexamxml_merlin"/options)"
fi

if ! command -v java > /dev/null; then
	ERROR "Java is not installed."
fi

if ! command -v python3.10 > /dev/null; then
	ERROR "Python 3.10 is not installed."
fi

for src in *.src
do
	test="${src%.*}"
	echo -n "$test"
	python3.10 parse.py < "$src" > "$test".your_out
	echo -n "$?" > "$test".your_rc
	diff "$test".rc "$test".your_rc > /dev/null \
	|| ERROR "Exit code differs, expected "$(cat "$test".rc)", got "$(cat "$test".your_rc)""
	[[ $(cat "$test".your_rc) != "0" ]] && OK && continue
	java -jar "$jexamxml"/jexamxml.jar "$test".out "$test".your_out "$test"_diff.xml  /D "$jexamxml"/options > /dev/null \
	|| ERROR "Output differs, see "$test"_diff.xml" \
	|| continue
	OK
done

# Restore old working directory
cd "$oldpwd"
