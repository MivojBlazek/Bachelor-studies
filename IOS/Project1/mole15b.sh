#!/bin/bash
POSIXLY_CORRECT=yes
DIR=$PWD
# MOLE_RC by mela byt cesta k souboru s logy .mole/list ale asi do ..../.config/mole_rc
EDITOR="vi"
VISUAL="vi"
m="false"
list="false"
secret="false"

function openFile () { # if soubor neexistuje
	EDIT_FILE="$FILE"
	EDIT_DATE="`date +%Y-%m-%d_%H-%M-%S`"
	if [ -z "$g_var" ]
	then
		echo "$EDIT_FILE:$g:$DIR:$EDIT_DATE" >> ~/.mole/list
	else
		echo "$EDIT_FILE::$DIR:$EDIT_DATE" >> ~/.mole/list
	fi
	$EDITOR -o "$DIR"/"$FILE" # Not everytime it must be 'vi'
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
	:
elif [ $1 = "list" ]
then
	list="true"
	shift 1
	if [ -z $1 ]
	then
		:
	elif [ $1 = "-m" ]
	then
		shift 1
	fi
elif [ $1 = "secret_log" ]
then
	secret="true"
	shift 1
	if [ $1 ]
	then
		if [ $1 = "-m" ]
		then
			shift 1
		fi
		if [ $1 = "-g" ]
		then
			shift 2
		fi
		if [ $1 = "-m" ] # nepovinny
		then
			shift 1
		fi
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
        m)      m="true";;
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
	# MOLE SECRET_LOG [DIR]
	if [ $secret = "true" ]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename"
		
		if [ -z $1 ] # MOLE SECRET_LOG
		then
			awk -F: '{print $3"/"$1, $4}' ~/.mole/list | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		elif [ -d $1 ] # MOLE SECRET_LOG DIR
		then
			PATH_F="$1"
			shift 1
			awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		fi
		bzip2 -z ~/.mole/"$filename"
		exit 0
	fi

	# MOLE [LIST] [-M] [-G]
	if [ -z $1 ]
	then
		if [ $list = "true" ] # MOLE LIST [-G]
		then
			if [ -z "$g" ] # MOLE LIST
			then
				sort ~/.mole/list | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
			else # MOLE LIST -G
				g_var=1
				awk -v gv="$g" -F: '{if($2 == gv) {print}}' ~/.mole/list | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
			fi
			exit 0
		fi

		if [ $m = "false" ] # MOLE [-G]
		then
			if [ -z "$g" ] # MOLE
			then
				FILE="`awk -F: '{print $1}' ~/.mole/list | awk 'END{print}'`"
			else # MOLE -G
				FILE="`awk -F: '{print $1}' ~/.mole/list | awk 'END{print}'`"
			fi
		fi

		if [ $m = "true" ] # MOLE -M [-G]
		then
			if [ -z "$g" ] # MOLE -M
			then
				g_var=1
				FILE="`awk -F: '{print $1}' ~/.mole/list | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			else # MOLE -M -G
				FILE="`awk -v gv="$g" -F: '{if($2 == gv) {print}}' ~/.mole/list | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			fi
		fi
	elif [ -d $1 ] # MOLE [LIST] [-M] [-G] DIR
	then
		PATH_F="$1"
		DIR="$1"
		shift 1
		if [ $list = "true" ] # MOLE LIST [-G] DIR
		then
			if [ -z "$g" ] # MOLE LIST DIR
			then
				awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
			else # MOLE LIST -G DIR
				g_var=1
				awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
			fi
			exit 0
		fi

		if [ $m = "false" ] # MOLE [-G] DIR
		then
			if [ -z "$g" ] # MOLE DIR
			then
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -F: '{print $1}' | awk 'END{print}'`"
			else # MOLE -G DIR
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: '{print $1}' | awk 'END{print}'`"
			fi
		else # MOLE -M [-G] DIR
			if [ -z "$g" ] # MOLE -M DIR
			then
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			else # MOLE -M -G DIR
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' ~/.mole/list | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			fi
		fi
	fi
else # MOLE [LIST] [SECRET_LOG] [-M] [-G] -AB [DIR]
	# vypocet -a, -b a ulozeni do filu
	nr=0
	sum_new_lines=1
	echo ";;;;;;;;;;" >> ~/.mole/list
	while read line
	do
		if [ $line = ";;;;;;;;;;" ]
		then
			break
		fi
		(( nr++ ))
		DATE_M=$(awk -v var=$nr -F: 'NR==var {print $4}' ~/.mole/list)

		dateCmp
		if [ "$?" -eq 0 ] # data se rovnaji -> kladny pripad -> zapsat
		then
			(( sum_new_lines++ ))
			if [ -z $g ] # MOLE [LIST] [SECRET_LOG] [-M] -AB [DIR]
			then
				if [ -z $1 ] # MOLE [LIST] [SECRET_LOG] [-M] -AB ??????????????????????????????
				then
					echo "$line" >> ~/.mole/list
				elif [ -d $1 ] # MOLE [LIST] [-M] -AB DIR ????????????????????????????????
				then
					PATH_F="$1"
					shift 1
					echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' >> ~/.mole/list
				fi
			else # MOLE [LIST] [-M] -G -AB [DIR]
				if [ -z $1 ] # MOLE [LIST] [-M] -G -AB ??????????????????????????????
				then
					echo "$line" | awk -v gv="$g" -F: '{if($2 == gv) {print}}' >> ~/.mole/list
				elif [ -d $1 ] # MOLE [LIST] [-M] -G -AB DIR ??????????????????????????????
				then
					PATH_F="$1"
					shift 1
					echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -v gv="$g" -F: '{if($2 == gv) {print}}' >> ~/.mole/list
				fi
			fi
		fi
	done < ~/.mole/list

	# mame ve filu uz odfiltrovano (filtrovalo se -G, -AB a DIR)
	# TEORETICKY BY UZ MEL BYT ODFILTROVANY I DIR!!!!!!!!!!
	
	if [ $list = "true" ] # MOLE LIST [-G] -AB [DIR]
	then
		if [ -z $1 ] # MOLE LIST [-G podle vrsku] -AB
		then
			awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
		elif [ -d $1 ] # MOLE LIST [-G podle vrsku] -AB DIR
		then
			PATH_F="$1"
			shift 1
			awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -v vpath="$PATH" -F: '{if ($3 == vpath) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | column -t
		fi
		
		# vymyzani zbytecneho obsahu v souboru protoze se ukonci
		(( nr++ ))
		echo "$(sed "$nr,$ d" ~/.mole/list)" > ~/.mole/list

		exit 0
	fi
	
	
	if [ $secret = "true" ] # MOLE SECRET_LOG -AB [DIR]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename"

		if [ -z $1 ] # MOLE SECRET_LOG -AB
		then
			awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		elif [ -d $1 ] # MOLE SECRET_LOG -AB DIR # zde pravdepodobne bude vice DIR
		then
			PATH_F="$1"
			shift 1
			awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -v vpath="$PATH" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		fi
		bzip2 -z ~/.mole/"$filename"

		# vymyzani zbytecneho obsahu v souboru protoze se ukonci
		(( nr++ ))
		echo "$(sed "$nr,$ d" ~/.mole/list)" > ~/.mole/list

		exit 0
	fi


	if [ -z $1 ] # MOLE [-M] [-G] -AB
	then
		if [ $m = "false" ] # MOLE [-G podle vrsku] -AB
		then
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -F: '{print $1}' | awk 'END{print}'`"
		else  # MOLE -M [-G podle vrsku] -AB
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
		fi
	elif [ -d $1 ] # MOLE [-M] [-G] -AB DIR
	then
		PATH_F="$1"
		shift 1
		if [ $m = "false" ] # MOLE [-G podle vrsku] -AB DIR
		then
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $1}' | awk 'END{print}'`"
		else
			FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' ~/.mole/list | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
		fi
	fi
	

	# vymyzani zbytecneho obsahu v souboru
	(( nr++ ))
	echo "$(sed "$nr,$ d" ~/.mole/list)" > ~/.mole/list
fi


if [[ ( -f $1 ) || ( $FILE ) ]] # MOLE [-M] [-G] [-AB] [DIR] FILE
then
	if [ -z $FILE ]
	then
		FILE="$1"
	fi
	openFile
	exit $?
fi
exit 1
