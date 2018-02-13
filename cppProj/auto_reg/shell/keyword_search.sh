CFG_FILE=$PWD/cfg/auto_reg.cfg
KEY_WORD=$1

TARGET=$(cat $CFG_FILE | grep $KEY_WORD)

echo "${TARGET#${KEY_WORD}=}"
