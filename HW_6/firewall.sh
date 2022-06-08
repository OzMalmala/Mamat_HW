#!/bin/bash

#filter packets
# reads the input and delete spaces
packets=$(cat | tr -d "[:blank:]");

#filter rules
#reads argument and delete spaces, empty lines, and lines starting with '#'
rules=$(grep -o "^[^#].*" $1 | sed 's/#.*/ /' | sed '/^$/d' | tr -d "[:blank:]");


#loop for all rules
for RULES in ${rules}
do
	# devide to rules parts
	IFS=',' read -r -a rules_parts <<< "$RULES"

	#run all packets
	echo "${packets}" | ./firewall.exe "${rules_parts[0]}" \
	  					  	   | ./firewall.exe "${rules_parts[1]}" \
	  					   	   | ./firewall.exe "${rules_parts[2]}" \
	  					  	   | ./firewall.exe "${rules_parts[3]}" \
	  					  	   | echo -e "$(cat)\n" \
	  					  	>> final.txt
done

sort -u final.txt | sed '/^$/d'
rm final.txt