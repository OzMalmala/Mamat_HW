#!/bin/bash
wget https://www.ynetnews.com/category/3082
grep -E -o -i "https://www.ynetnews.com/article/([0-9]|[a-z])*" 3082 | sort | uniq > url_list.txt
echo $(wc -l < url_list.txt) > results.csv
while read -r line; do
	wget -O cur_article $line 
	bibi=$(grep -o 'Netanyahu' cur_article | wc -l)
	benet=$(grep -o 'Bennett' cur_article | wc -l)
	echo "$line, Netanyahu,$bibi, Bennett,$benet " >> results.csv
done < url_list.txt