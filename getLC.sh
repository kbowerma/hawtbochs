date "+%H:%M:%S " |xargs printf >> data.txt ;printf ',' >> data.txt;  particle get LakeCreek voltage | xargs printf >> data.txt ; printf ','  >> data.txt;  particle get LakeCreek soc  >> data.txt
