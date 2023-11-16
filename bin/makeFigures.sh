#!/bin/bash
if [[ $# -lt "1" ]];then
cat<<ENDOFUSAGE
USAGE: 
 ${0} ResultFile
 ${0} -M RATIO ResultFile
 
DESCRIPTION:
 This bash script goes through the result files of Doris and generates raster images using cpxfiddle.
 If RATIO of azimuth lines to range pixels is not specified 1/5 is used as default (OK for ERS, Envisat). 
 This is used for the -M parameter of cpxfiddle.

Examples:
 ${0} -M 1/5 */*.res
ENDOFUSAGE
exit 1
fi

rm -rf temporary.file

if [[ ${1} == "-M" ]];then
	ratio=${2}
	lc=2
else
	ratio="1/5"
	lc=1
fi
echo "Using Ratio:${ratio}"

c=0
for parameter in "$@"
do

if [[ ${c} -lt ${lc} ]];then
	c=$((c+1))
	continue;
fi
#echo ${parameter}
grep -n output_file ${parameter} |tr -d "\t" | tr -d " " > temporary.file
	for file in `cat temporary.file`
	do
		fname=`echo ${file}| cut -d":" -f3`
		if [[ -e ${fname} ]];then
			echo "Generating ${fname}"
			bname=`basename ${fname}`	
		else 
			echo "File not found: *${fname}*"
			continue
		fi
		width=`grep -m1 -A10 ${fname} ${parameter} | grep "Number of pixels" | cut -f2 -d":" | tr -d "\t"| tr -d " "`
		if [ -z ${width} ];then
			echo "Unknown width for: ${fname}"
			continue
		fi
		
		format=`grep -m1 -A3 ${fname} ${parameter} | grep format| cut -f2 -d":" | tr -d "\t"| tr -d " "`
		if [ ${format} == "complex_short" ]
		then
			frmt="ci2"
			cpxfiddle -w ${width} -f ${frmt} -e 0.5 -s 1.2 -q mixed -o sunraster -c cool -M${ratio} ${fname} > ${bname}_mixed.ras
			#`cmd`
			cpxfiddle -w ${width} -f ${frmt} -e 0.5 -s 1.0 -q mag  -o sunraster -c gray -M${ratio} ${fname} > ${bname}_mag.ras
			#`cmd`
			cpxfiddle -w ${width} -f ${frmt} -q phase  -o sunraster -c jet -M${ratio} ${fname} > ${bname}_pha.ras
			#`cmd`			
		elif [ ${format} == "complex_real4" ]
		then
			frmt="cr4"
			cpxfiddle -w ${width} -f ${frmt} -e 0.3 -s 1.2 -q mixed -o sunraster -c jet -M${ratio} ${fname} > ${bname}_mixed.ras
			#echo $cmd
			cpxfiddle -w ${width} -f ${frmt} -e 0.3 -s 1.0 -q mag  -o sunraster -c gray -M${ratio} ${fname} > ${bname}_mag.ras
			#echo $cmd
			cpxfiddle -w ${width} -f ${frmt} -q phase  -o sunraster -c jet -M${ratio} ${fname} > ${bname}_pha.ras
			#echo $cmd					
		elif [ ${format} == "real4" ]
		then
			frmt="r4"
			cpxfiddle -w ${width} -f ${frmt} -q normal  -o sunraster -c gray -M${ratio} ${fname} > ${bname}.ras
			#echo $cmd					
		fi
		wait
	done
done

