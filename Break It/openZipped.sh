#!/bin/bash

for compr in $(ls -1 *.zip); do
	dir=${compr%.zip}
	mkdir $dir
	unzip $compr -d $dir
done

for compr in $(ls -1 *.tar.gz); do
	dir=${compr%.tar.gz}
	mkdir $dir
	tar -xzvf $compr -C $dir
done

for compr in $(ls -1 *.tar); do
	dir=${compr%.tar}
	mkdir $dir
	tar -xvf $compr -C $dir
done

