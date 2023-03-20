#!/bin/sh
#
#	MOLE
#	bonus parameter -d added
#	login: xblaze38
#	18.03.2023
#
export POSIXLY_CORRECT=yes


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

d="false"
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
		err_msg=$?
		if [ "$err_msg" -ne 0 ]
		then
			echo "Error with editor!"
			exit $err_msg
		fi
	else
		vi -o "$DIR"/"$FILE"
		err_msg=$?
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


while getopts :hmdg:b:a: o
do
	case "$o" in
	h)	printf "
'mole' is a wrapper pro effective using text editor with option to select the most modified file or last modified file.

Usage:
	mole -h                                ---opens help menu
	mole [-g GROUP] FILE                   ---opens FILE
	mole [-m] [-d] [FILTERS] [DIRECTORY]   ---opens file randomly or according to filters
	mole list [-d] [FILTERS] [DIRECTORY]   ---displays a list of files that were edited

Arguments:
	[-g GROUP] assigns file opening to GROUP
	[-m] chooses the most edited file to open, if there are 2 or more files, it chooses one of them randomly, if there is no file edited yet, it writes error
	[-d] chooses modified files that are not assign to any group (incompatible with parameter -g GROUP)
	[DIRECTORY] default is current directory
	[FILTERS] applies following search filters:
		[-g GROUP1[,GROUP2[,...]]] file will only be considered if its execution falls into at least one GROUP (incompatible with parameter -d)
		[-a DATE] Records of edited files before this date will not be considered (DATE format is YYYY-MM-DD)
		[-b DATE] Records of edited files after this date will not be considered (DATE format is YYYY-MM-DD)\n"
		exit 0;;
	m)	m="true";;
	d)	d="true";;
	g)	g="$OPTARG";;
	b)	DATE_B="$OPTARG";;
	a)	DATE_A="$OPTARG";;
	*)	OPTIND=$((OPTIND-1))
		break;;
	esac
done
OPTIND=$((OPTIND-1))
shift $OPTIND


if [ $d = "true" ] && [ "$g" ]
then
	echo "-g is not compatible with -d" 1>&2
	exit 3
fi

if [ -z $DATE_A ] && [ -z $DATE_B ]
then
	# MOLE SECRET_LOG [DIR]
	if [ $secret = "true" ]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename".bz2
		
		if [ -z "$1" ] # MOLE SECRET_LOG
		then
			awk -F: '{print $3"/"$1, $4}' $MOLE_RC | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' | bzip2 -z > ~/.mole/"$filename".bz2
		fi
		while [ -d "$1" ] # MOLE SECRET_LOG DIR
		do
			PATH_F="$1"
			shift 1
			awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' | bzip2 -z >> ~/.mole/"$filename".bz2
		done
		exit 0
	fi

	# MOLE [LIST] [-M] [-G]
	if [ -z $1 ]
	then
		if [ $list = "true" ] # MOLE LIST [-G]
		then
			if [ -z "$g" ] # MOLE LIST
			then
				if [ $d = "true" ]
				then
					awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -v vpath="$DIR" -F: '{if ($3 == vpath) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | awk -v FS=', ' -v OFS=',' '$1=$1' | column -t
				else
					awk -v vpath="$DIR" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | awk -v FS=', ' -v OFS=',' '$1=$1' | column -t
				fi
			else # MOLE LIST -G
				g_var=1
				awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | awk -v vpath="$DIR" -F: '{if ($3 == vpath) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | column -t
			fi
			exit 0
		fi

		if [ $m = "false" ] # MOLE [-G] ?????????????????? chybi DIR= jakoby kdyz posledni/nejcasteji bude v jine slozce, nez jsme ted my
		then
			if [ $d = "true" ] # MOLE
			then
				DIR="`awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -F: '{print $3}' | awk 'END{print}'`"
				FILE="`awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -F: '{print $1}' | awk 'END{print}'`"
			else # MOLE [-G]
				DIR="`awk -F: '{print $3}' $MOLE_RC | awk 'END{print}'`"
				FILE="`awk -F: '{print $1}' $MOLE_RC | awk 'END{print}'`"
			fi
		fi

		if [ $m = "true" ] # MOLE -M [-G] ?????????????????? chybi DIR= jakoby kdyz posledni/nejcasteji bude v jine slozce, nez jsme ted my
		then
			if [ -z "$g" ] # MOLE -M
			then
				if [ $d = "true" ]
				then
					FILE="`awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
				else
					FILE="`awk -F: '{print $1}' $MOLE_RC | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
				fi
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
				if [ $d = "true" ]
				then
					awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | awk -v FS=', ' -v OFS=',' '$1=$1' | column -t
				else
					awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | awk -v FS=', ' -v OFS=',' '$1=$1' | column -t
				fi
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
				if [ $d = "true" ]
				then
					FILE="`awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $1}' | awk 'END{print}'`"
				else
					FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $1}' | awk 'END{print}'`"
				fi
			else # MOLE -G DIR
				FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: '{print $1}' | awk 'END{print}'`"
			fi
		else # MOLE -M [-G] DIR
			if [ -z "$g" ] # MOLE -M DIR
			then
				if [ $d = "true" ]
				then
					FILE="`awk -F: '{if($2 == "") {print}}' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
				else
					FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: '{print $1}' | tr -c '[:alnum:]' '[\n*]' | sort | uniq -c | sort -nr | head  -1 | awk '{print $2}'`"
				fi
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
				if [ $d = "true" ]
				then
					if [ "$PATH_F" ] && [ $secret = "false" ]
					then
						echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{if($2 == "") {print}}' >> "$MOLE_RC"
					else
						echo "$line" | awk -F: '{if($2 == "") {print}}' >> $MOLE_RC
					fi
				else
					if [ "$PATH_F" ] && [ $secret = "false" ]
					then
						echo "$line" | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' >> "$MOLE_RC"
					else
						echo "$line" >> $MOLE_RC
					fi
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
		awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -v vpath="$DIR" -F: '{if ($3 == vpath) {print}}' | sort | awk -F: '{print $1, $2}' | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i","; j++} NF=j}1' | awk '{if($2 == "") $2="-"}1' | awk -v FS=', ' -v OFS=',' '$1=$1' | column -t
		
		# vymyzani zbytecneho obsahu v souboru protoze se ukonci
		nr=$((nr+1))
		echo "$(sed "$nr,$ d" "$MOLE_RC")" > "$MOLE_RC"

		exit 0
	fi
	
	
	if [ $secret = "true" ] # MOLE SECRET_LOG -AB [DIR]
	then
		DATETIME="`date +%Y-%m-%d_%H-%M-%S`"
		filename="log_"$USER"_"$DATETIME""
		touch ~/.mole/"$filename".bz2

		if [ "$PATH_F" ]
		then
			awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' | bzip2 -z > ~/.mole/"$filename".bz2
		else
			awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' | bzip2 -z > ~/.mole/"$filename".bz2
		fi
		
		while [ -d "$1" ] # MOLE SECRET_LOG -AB DIR [DIR]
		do
			PATH_F="$1"
			shift 1
			awk '/;;;;;;;;;;/{flag=1;next}flag' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | awk -F: '{print $3"/"$1, $4}' | sort | awk '!seen[$0]++' | awk '$1!=p{if(p)print s; p=$1; s=$0; next}{sub(p,x); s=s $0} END{print s}' | awk '{for(i=j=2; i < NF; i=i+1) {$j = $i; j++} NF=j}1' | awk -v OFS=';' '$1=$1' | bzip2 -z >> ~/.mole/"$filename".bz2
		done

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


if [ -f $1 ] || [ $FILE ] # MOLE [-M] [-G] [-AB] [DIR] FILE
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

	if [ "$1" ] && [ -f $1 ] 
	then
		:
	else # je to druhy prikaz -> kontrola zda neni posledni smazany

		# ulozeni posledniho prvku, ktery by se normalne vybral
		if [ "$g" ]
		then
			if [ -z $PATH_F ]
			then
				NumberOfRows="$(awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | wc -l)"
			else
				NumberOfRows="$(awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' | wc -l)"
			fi
		else
			if [ -z $PATH_F ]
			then
				NumberOfRows="$(awk 'END{print FNR}' $MOLE_RC)"
			else
				NumberOfRows="$(awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | wc -l)"
			fi
		fi

		DIR_FILE="$DIR"/"$FILE"
		while [ ! -f "$DIR_FILE" ]
		do
			NumberOfRows=$((NumberOfRows-1))
			if [ $NumberOfRows -lt 1 ]
			then
				echo "All edited files with these filters were probably deleted!" 1>&2
				exit 3
			fi
			
			if [ "$g" ]
			then
				if [ -z "$PATH_F" ]
				then
					DIR_FILE="`awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $3"/"$1}'`"
					FILE="`awk -v gv="$g" -F: '{if($2 == gv) {print}}' $MOLE_RC | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $1}'`"
				else
					DIR_FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $3"/"$1}'`"
					FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -v gv="$g" -F: '{if($2 == gv) {print}}' | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $1}'`"
				fi
			else
				if [ -z "$PATH_F" ]
				then
					DIR_FILE="`awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $3"/"$1}' $MOLE_RC`"
					FILE="`awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $1}' $MOLE_RC`"
				else
					DIR_FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $3"/"$1}'`"
					FILE="`awk -v vpath="$PATH_F" -F: '{if ($3 == vpath) {print}}' $MOLE_RC | awk -F: -v nrnew="$NumberOfRows" 'NR==nrnew {print $1}'`"
				fi
			fi
		done
	fi

	openFile
	exit $?
fi

echo "There is no file to select!" 1>&2
exit 1
