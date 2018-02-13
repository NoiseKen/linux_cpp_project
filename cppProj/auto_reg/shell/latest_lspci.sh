LSPCI_LOG=$PWD/lspci.log
#LSPCI_LOG=$1/lspci.log
BSF=$($PWD/shell/keyword_search.sh PCI_BSF)

lspci -s $BSF -vvv > $LSPCI_LOG
