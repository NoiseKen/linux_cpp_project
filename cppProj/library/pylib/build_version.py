#!/usr/bin/python2.7
import sys
import os
import time
import ansi

VersionDefine=[  ('#define MAJOR_VER',  'for major version number')
                ,('#define MINOR_VER',  'for minor version number')
                ,('#define RELEASED ',  'for rleased number')
                ,('#define BUILD_VER',  'for build number')
                ]

def print_fail(msg):
    print ansi.Style.BRIGHT
    print ansi.Fore.RED
    print msg
    print ansi.Style.RESET_ALL

def useage():
    print ansi.Style.BRIGHT
    print ansi.Fore.MAGENTA
    print 'Usage : %s <fille> [+[+[+]]]' % __file__
    print ansi.Fore.GREEN
    print 'file  : search file for version define(*)'
    print '+     : increase release number, will clear build number to zero'
    print '++    : increase minor number, will clear release/ build number to zero'
    print '+++   : increase major number, will clear minor/ release/ build number to zero'
    print ansi.Fore.BLUE
    print '(*) Version Define list below'
    for define, doc in VersionDefine:
        print '%s%s%s : %s' % (ansi.Fore.CYAN, define, ansi.Fore.WHITE, doc)
    print ansi.Style.RESET_ALL

def modify_c_version_define(fn, incIdx):
    incKey=VersionDefine[incIdx][0]
    with open( fn, 'r+w' ) as f:
        lines=f.readlines()
        found=False
        for no, l in enumerate(lines):
            if l.startswith(incKey):
                #find key
                #print 'Found key[%s] in file[%s] line[%d] ==> %s' % (incKey, fn, no, l.strip('\n'))
                #keep original string
                rawVal=eval(l[len(incKey):-1].strip(' \t'))
                print '%s was %d' % (incKey, rawVal)
                lines[no]='%s    %d\n' % (incKey, rawVal+1)
                print '%s %sis%s  %d' % (incKey, (ansi.Style.BRIGHT+ansi.Fore.CYAN), ansi.Style.RESET_ALL, rawVal+1)
                found=True
                break

        #check zero necessary
        while incIdx!=(len(VersionDefine)-1):
            incIdx+=1
            incKey=VersionDefine[incIdx][0]
            for no, l in enumerate(lines):
                if l.startswith(incKey):
                    #find key
                    #print 'Found key[%s] in file[%s] line[%d] ==> %s' % (incKey, fn, no, l.strip('\n'))
                    #zerolize
                    rawVal=eval(l[len(incKey):-1].strip(' \t'))
                    print '%s was %d' % (incKey, rawVal)
                    lines[no]='%s    %d\n' % (incKey, 0)
                    print '%s %sis%s  %d' % (incKey, (ansi.Style.BRIGHT+ansi.Fore.CYAN), ansi.Style.RESET_ALL, 0)
                    break

        if found:
            f.seek(0, os.SEEK_SET)
            f.write(''.join(lines))           

# direct execute test code
if __name__ == '__main__':
    if len(sys.argv)>1:

        fn=sys.argv[1]
        if not os.path.isfile(fn):
            print_fail('file [%s] not exist!!' % fn)
            sys.exit(-1)

        mode=len(VersionDefine)-1
        if len(sys.argv)>2:
            modesDict={'+':len(VersionDefine)-2, '++':len(VersionDefine)-3, '+++':len(VersionDefine)-4}
            if sys.argv[2] in modesDict.keys():
                mode=modesDict[sys.argv[2]]

        #print mode[0]
        modify_c_version_define(fn, mode)
        
        #clear pyc file
        root=os.popen('git rev-parse --show-toplevel').read().strip('\n')
        cli='find %s -name "*.pyc" -exec rm -f {} \;' % root
        os.system(cli)
    else:
        useage()

