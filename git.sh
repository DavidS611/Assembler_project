#!/bin/bash

# Git target
git:
	git add "$1"
	# optional body comment
	if [[ -n "$3" ]]; then
		git commit -m "$2" -m "$3"
	else
		git commit -m "$2"
	fi
	git push -u origin master
