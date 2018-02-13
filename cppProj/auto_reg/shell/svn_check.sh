#common head
. $PWD/shell/common


#shell start
shell_running


#svn_folder="./fwbin"
svn_folder=$1
svn_repo=$($PWD/shell/keyword_search.sh SVN_REPO)
svn_user=$($PWD/shell/keyword_search.sh SVN_USER)
svn_paswd=$($PWD/shell/keyword_search.sh SVN_PASWD)


echo "target repo=$svn_repo"
#chek if local_folder not exist check out 1st version fwbinary
if [ ! -d "$svn_folder" ]
then new_svn_folder=1
	svn co "$svn_repo" $svn_folder --username $svn_user --password $svn_paswd >> $SH_RST
	shell_exit_if_error $? "can NOT check out SVN[${svn_repo}]"	
else
	new_svn_folder=0
fi

check_local_ver=$(svn info $svn_folder | grep "Last Changed Rev:")
check_repo_ver=$(svn info "$svn_repo" --username $svn_user --password $svn_paswd | grep "Last Changed Rev:")

shell_exit_if_error $? "can NOT check out SVN[${svn_repo}]"

if [ $new_svn_folder -eq 1 ]
then local_ver=0
else local_ver="${check_local_ver#Last Changed Rev:}"
fi
repo_ver="${check_repo_ver#Last Changed Rev:}"


echo "svn repo=${svn_repo}"	 >> $SH_RST
echo "local ver = $local_ver" >> $SH_RST
echo "repo ver = $repo_ver" >> $SH_RST



if [ $repo_ver -gt $local_ver ]
then echo "repo version > local version"	>> $SH_RST
	echo "check out new version bin file!!" >> $SH_RST
	echo "svn co $svn_repo $svn_folder" >> $SH_RST; svn co "$svn_repo" $svn_folder --username $svn_user --password $svn_paswd >> $SH_RST
	shell_exit_if_error $? "can NOT check out SVN[$svn_repo]"
else echo "keep local fw" >> $SH_RST
	echo "-88888" >> $SH_RST
fi

echo $local_ver > svninfo

shell_exit
