#!/bin/sh
POSIXLY_CORRECT=yes
# BASH !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! pouzivani $(XXX) ma pry side efekt, ze se vymaze koncovy newline z vysledku
DIR=$PWD
if [ -z "$MOLE_RC" ]
then
	echo "Variable MOLE_RC is not set!" 1>&2
	exit 2
elif [ -f "$MOLE_RC" ]
then
	:
else
	touch "$MOLE_RC"
fi

m="false"
list="false"
secret="false"

openFile() {
	EDIT_FILE="$FILE"
	EDIT_DATE="`date +%Y-%m-%d_%H-%M-%S`"
	if [ -z "$g_var" ]
	then
		echo "$EDIT_FILE:$g:$DIR:$EDIT_DATE" >> $MOLE_RC
	else
		echo "$EDIT_FILE::$DIR:$EDIT_DATE" >> $MOLE_RC
	fi
	if [ "$EDITOR" ]
	then
		$EDITOR -o "$DIR"/"$FILE"
		err_msg=$?
		if [ "$err_msg" -ne 0 ]
		then
			echo "Error with editor!"
			exit $err_msg
		fi
	elif [ "$VISUAL" ]
	then
		$VISUAL -o "$DIR"/"$FILE"
		if [ "$err_msg" -ne 0 ]
		then
			echo "Error with editor!"
			exit $err_msg
		fi
	else
		vi -o "$DIR"/"$FILE"
		if [ "$err_msg" -ne 0 ]
		then
			echo "Error with editor!"
			exit $err_msg
		fi
	fi
	return 0
}

dateCmp() {
	DATE_MID=$(echo "$DATE_M" | cut -d'_' -f 1)
	middle_date="$(date -d $DATE_MID +%s)"
	if [ $DATE_B ]
	then
		to_date="$(date -d $DATE_B +%s)"
		if [ $middle_date -gt $to_date ]
		then
			return 1
		fi
	fi
	if [ $DATE_A ]
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
		if [ $1 = "-m" ]
		then
			shift 1
		fi
	fi
fi


while getopts :hmg:b:a: o
do
	case "$o" in
	h)	printf "
'mole' is a wrapper pro effective using text editor with option to select the most modified file or last modified file.

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
	m)	m="true";;
	g)	g="$OPTARG";;
	b)	DATE_B="$OPTARG";;
	a)	DATE_A="$OPTARG";;
	*)	OPTIND=$((OPTIND-1))
		break;;
	esac
done
OPTIND=$((OPTIND-1))
shift $OPTIND


if [ -z $DATE_A -a -z $DATE_B ]
then
	# MOLE SECRET_LOG [DIR]
	if [ $secret = "true" ]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename"
		
		if [ -z $1 ] # MOLE SECRET_LOG
		then
			awk -F: '{print $3"/"$1, $4}' $MOLE_RC | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		elif [ -d $1 ] # MOLE SECRET_LOG DIR
		then
			PATH_F="$1"
			shift 1
			awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
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
				sort "$MOLE_RC" | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
			else # MOLE LIST -G
				g_var=1
				awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
			fi
			exit 0
		fi

		if [ $m = "false" ] # MOLE [-G]
		then
			if [ -z "$g" ] # MOLE
			then
				DIR="`awk -F: '{print $3}' $MOLE_RC | awk 'END{print}'`"
				FILE="`awk -F: '{print $1}' $MOLE_RC | awk 'END{print}'`"
			else # MOLE -G
				DIR="`awk -F: '{print $3}' $MOLE_RC | awk 'END{print}'`"
				FILE="`awk -F: '{print $1}' $MOLE_RC | awk 'END{print}'`"
			fi
		fi

		if [ $m = "true" ] # MOLE -M [-G] ?????????????????? chybi DIR= jakoby kdyz posledni/nejcasteji bude v jine slozce, nez jsme ted my
		then
			if [ -z "$g" ] # MOLE -M
			then
				FILE="`awk -F: '{print $1}' $MOLE_RC | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			else # MOLE -M -G
				FILE="`awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			fi
		fi
	elif [ -d $1 ] # MOLE [LIST] [-M] [-G] DIR
	then
		PATH_F="$1"
		DIR="$PATH_F"
		shift 1
		if [ $list = "true" ] # MOLE LIST [-G] DIR
		then
			if [ -z "$g" ] # MOLE LIST DIR
			then
				awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
			else # MOLE LIST -G DIR
				g_var=1
				awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
			fi
			exit 0
		fi

		if [ $m = "false" ] # MOLE [-G] DIR
		then
			if [ -z "$g" ] # MOLE DIR
			then
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $1}' | awk 'END{print}'`"
			else # MOLE -G DIR
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: '{print $1}' | awk 'END{print}'`"
			fi
		else # MOLE -M [-G] DIR
			if [ -z "$g" ] # MOLE -M DIR
			then
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			else # MOLE -M -G DIR
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
			fi
		fi
	fi
else # MOLE [LIST] [SECRET_LOG] [-M] [-G] -AB [DIR]
	if [ -z $1 ]
	then
		:
	elif [ -d $1 ] # DIR mimo while aby se provedl jen jednou
	then
		PATH_F="$1"
		DIR="$PATH_F"
		shift 1
	fi
	# vypocet -a, -b a ulozeni do filu
	nr=0
	sum_new_lines=1
	echo ";;;;;;;;;;" >> "$MOLE_RC"
	while read line
	do
		if [ "$line" = ";;;;;;;;;;" ]
		then
			break
		fi
		nr=$((nr+1))
		DATE_M=$(awk -v var=$nr -F: 'NR==var {print $4}' $MOLE_RC)

		dateCmp
		if [ "$?" -eq 0 ] # data se rovnaji -> kladny pripad -> zapsat
		then
			sum_new_lines=$((sum_new_lines+1))
			if [ -z $g ] # MOLE [LIST] [SECRET_LOG] [-M] -AB [DIR]
			then
				if [ "$PATH_F" ]
				then
					echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' >> "$MOLE_RC"
				else
					echo "$line" >> $MOLE_RC
				fi
			else # MOLE [LIST] [-M] -G -AB [DIR]
				if [ "$PATH_F" ] # MOLE [LIST] [-M] -G -AB DIR 
				then
					echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -v gv="$g" -F: '{if($2 == gv) {print}}' >> "$MOLE_RC"
				else
					echo "$line" | awk -v gv="$g" -F: '{if($2 == gv) {print}}' >> "$MOLE_RC"
				fi
			fi
		fi
	done < "$MOLE_RC"

	# mame ve filu uz odfiltrovano (filtrovalo se -G, -AB a DIR)
	
	if [ $list = "true" ] # MOLE LIST [-G] -AB [DIR]
	then
		awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
		
		# vymyzani zbytecneho obsahu v souboru protoze se ukonci
		nr=$((nr+1))
		echo "$(sed "$nr,$ d" "$MOLE_RC")" > "$MOLE_RC"

		exit 0
	fi
	
	
	if [ $secret = "true" ] # MOLE SECRET_LOG -AB [DIR]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename"

		awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' > ~/.mole/"$filename"
		bzip2 -z ~/.mole/"$filename"

		# vymyzani zbytecneho obsahu v souboru protoze se ukonci
		nr=$((nr+1))
		echo "$(sed "$nr,$ d" "$MOLE_RC")" > "$MOLE_RC"

		exit 0
	fi


	if [ $m = "false" ] # MOLE [-G podle vrsku] -AB
	then
		FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -F: '{print $1}' | awk 'END{print}'`"
	else  # MOLE -M [-G podle vrsku] -AB
		FILE="`awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
	fi
	

	# vymyzani zbytecneho obsahu v souboru
	nr=$((nr+1))
	echo "$(sed "$nr,$ d" "$MOLE_RC")" > "$MOLE_RC"
fi


if [ -f $1 -o $FILE ] # MOLE [-M] [-G] [-AB] [DIR] FILE
then
	if [ -z $FILE ]
	then
		FILE="$1"
	fi

	FILE_TMP="`echo "$FILE" | awk -F/ '{print $NF}'`"
	DIR_NEXT="`echo "$FILE" | awk -F/ '(NF=NF-1) (OFS="/") {print}'`"
	if [ "$DIR_NEXT" ]
	then
		if [ -z $DIR ]
		then
			DIR="$DIR_NEXT"
		else
			DIR="$DIR"/"$DIR_NEXT"
		fi
	fi
	FILE="$FILE_TMP"
	openFile
	exit $?
fi

echo "There is no file to select!" 1>&2
exit 1
