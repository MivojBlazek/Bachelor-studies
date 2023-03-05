#!/bin/bash
POSIXLY_CORRECT=yes
DIR=$PWD
# MOLE_RC by mela byt cesta k souboru s logy .mole/list ale asi do ..../.config/mole_rc
EDITOR="vi"
VISUAL="vi"
m="false"
list="false"
secret="false"

function openFile () {
	EDIT_FILE="$FILE"
	EDIT_DATE="`date +%Y-%m-%d_%H-%M-%S`"
	if [ -z "$g_var" ]
	then
		echo "$EDIT_FILE:$g:$DIR:$EDIT_DATE" >> /home/"$USER"/.mole/list
	else
		echo "$EDIT_FILE::$DIR:$EDIT_DATE" >> /home/"$USER"/.mole/list
	fi
	$EDITOR -o "$FILE" # Not everytime it must be 'vi'
	return 0
}

function dateCmp () {
	DATE_MID=$(echo "$DATE_M" | cut -d'_' -f 1)
	middle_date="$(date -d $DATE_MID +%s)"
	if [[ ! ( -z $DATE_B ) ]] #if [ $DATE_B ]     u A taky
	then
		to_date="$(date -d $DATE_B +%s)"
		if [ $middle_date -gt $to_date ]
		then
			return 1
		fi
	fi
	if [[ ! ( -z $DATE_A ) ]]
	then
		from_date="$(date -d $DATE_A +%s)"
		if [ $middle_date -lt $from_date ]
		then
			return 1
		fi
	fi
	return 0
}



if [ -z $1 ]
then
	echo "only mole" # pry se muze pozit ':' a nebude to delat nic
elif [ $1 = "list" ]
then
	echo "$1 is 'list'"
	list="true"
	shift 1
	if [ -z $1 ]
	then
		echo "prazdny list"
	elif [ $1 = "-m" ]
	then
		shift 1
	fi
elif [ $1 = "secret_log" ]
then
	echo "$1 is 'secret_log'"
	secret="true"
	shift 1
	if [ $1 = "-m" ]
	then
		shift 1
	fi
	if [ $1 = "-g" ]
	then
		shift 2
	fi
fi


while getopts :hmg:b:a: o
do
	case "$o" in
        h)      printf "
Usage:
	mole -h                           ---opens help menu
	mole [-g GROUP] FILE              ---opens FILE
	mole [-m] [FILTERS] [DIRECTORY]   ---opens file randomly or according to filters
	mole list [FILTERS] [DIRECTORY]   ---displays a list of files that were edited

Arguments:
	[-g GROUP] assigns file opening to GROUP
	[-m] chooses the most edited file to open, if there are 2 or more files, it chooses one of them randomly, if there is no file edited yet, it writes error
	[DIRECTORY] default is current directory
	[FILTERS] applies following search filters:
		[-g GROUP1[,GROUP2[,...]]] file will only be considered if its execution falls into at least one GROUP
		[-a DATE] Records of edited files before this date will not be considered (DATE format is YYYY-MM-DD)
		[-b DATE] Records of edited files after this date will not be considered (DATE format is YYYY-MM-DD)\n"
		exit 0;;
        m)      m="true"
		FILE="`awk -F: '{print $1}' .mole/list | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`";; # WORKS but only still 'mole -m'
        g)	g="$OPTARG";;
	b)	DATE_B="$OPTARG";;
	a)	DATE_A="$OPTARG";;
	*)      ((OPTIND--))
		break;;
        esac
done
((OPTIND--))
shift $OPTIND


if [[ ( -z $DATE_A ) && ( -z $DATE_B ) ]]
then
	if [ -z $1 ]
	then
		if [ $m = "false" ]
		then
			FILE="`awk -F: '{print $1}' .mole/list | awk 'END{print}'`"
		fi
		if [ $list = "true" ]
		then
			# samotny list -> zobraz vse
			sort .mole/list | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
			#sort .mole/list | awk -F: 'BEGIN{OFS=": "} {print $1, $2}' | column -t
			exit 0
		fi
	elif [ -d $1 ]
	then
		DIR="$1"
		shift 1
		if [[ ( -z $1 ) && ( $m = "false" ) ]] # -m tady nebude fungovat
		then
			FILE="`awk -F: '{print $1}' .mole/list | awk 'END{print}'`"
		fi
	fi
else
	nr=0
	sum_new_lines=1
	echo ";;;;;;;;;;" >> /home/"$USER"/.mole/list
	while read line
	do
		if [ $line = ";;;;;;;;;;" ]
		then
			break
		fi
		(( nr++ ))
		DATE_M=$(awk -v var=$nr -F: 'NR==var {print $4}' ~/.mole/list)

		dateCmp
		if [ "$?" -eq 0 ]
		then
			(( sum_new_lines++ ))
			if [ -z $g ]
			then
				echo "$line" >> /home/"$USER"/.mole/list
			else
				g_var=1
				echo "$line" | awk -v gv="$g" -F: '{if($2 == gv) {print}}' >> /home/"$USER"/.mole/list
			fi
		fi
	done < /home/"$USER"/.mole/list

	if [[ ( -z $1 ) && ( $list = "false" ) ]]
	then
		if [ $m = "false" ]
		then
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' .mole/list | awk -F: '{print $1}' .mole/list | awk 'END{print}'`"
		fi
	elif [[ ( -d $1 ) && ( $list = "false" ) ]]
	then
		DIR="$1"
		shift 1
		if [[ ( -z $1 ) && ( $m = "false" ) ]] # -m tady nebude fungovat
		then
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' .mole/list | awk -F: '{print $1}' .mole/list | awk 'END{print}'`"
		fi
	fi
	
	#'list' part
	if [ $list = "true" ]
	then
		#uzivatel zadal list a '-a' || '-b'
		awk '/;;;;;;;;;;/{flag=1;next}flag' .mole/list | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
	fi


	# vymyzani zbytecneho obsahu v souboru
	(( nr++ ))
	echo "$(sed "$nr,$ d" .mole/list)" > /home/"$USER"/.mole/list
fi


if [[ ( -f $1 ) || ( $FILE ) ]]
then
	if [ -z $FILE ]
	then
		FILE="$1"
	fi
	openFile
	exit $?
fi
