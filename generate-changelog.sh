#!/bin/bash
#
# Generate markdown ChangeLog files based on git history.

tmp=$(mktemp)
touch ${tmp}

# Commits prior to this one contain mangled commit history.
# Reformatting these requires some extra handholding
LASTCOMMIT="2d48e88ad67f600376926f073302b2617e49675c"

prev_tag="$LASTCOMMIT"
echo "# ChangeLog"
for tag in $(git tag -l [0-9]*.*[0-9] | sort -V); do
	# Prepend tags to the output because we want the ChangeLog in the
	# reverse order, e.g. 2.0.0, 1.0.1 and 1.0.0.
	{
		date=$(git show --date=short --format='tformat:%cd' -s ${tag} | tail -n 1)
		echo -e "\n## ${tag} - ${date}\n"
		git --no-pager log --no-merges --format='format: - %s%n' \
		    ${prev_tag:+${prev_tag}..}${tag} | cat - ${tmp}
	} > ${tmp}.1
	mv ${tmp}.1 ${tmp}
	prev_tag=${tag}
done

# Send the output to stdout.
cat ${tmp}

# Append a revised changelog for stuff prior to LASTCOMMIT in
# consistent style with the generated comments
cat ChangeLog.old.md

rm ${tmp}
