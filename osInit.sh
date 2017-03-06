#!/bin/bash

function verbose(){
    if [ "$DEBUG" == 'yes' ];then 
        echo $@
    fi
}

function lwarn(){
    #33m,yellow
    echo -e "\033[1;33m" "$@" "\033[0m"
}

function lerror(){
    #31m,red
    echo -e "\033[1;31m" "$@" "\033[0m"
    exit -1
}

function lsudo(){
    echo -e "\033[1;32msudo for:"\""$@"\""\033[0m"
    sudo "$@"
    if [ $? -eq 1 ];then
        echo -e "\033[1;32m""sudo authen error exit -1""\033[0m"
        exit -1
    fi
}

function callFunc(){
    verbose "=================func:$1 in========================"
    $1
    local ret=$?
    verbose "=================func:$1 finished=================="
    return $ret
}

#$1,pkg name
#$2==noInstall,do not install else other repos...
#$.==enable repo
function pkgCheckInstall(){
    local pkg=$1
    count=`rpm -qa "$pkg" | grep -c "$pkg"`
    if [ $count -gt 0 ];then
        verbose "$pkg has been installed,return"
        return 0    
    else
        if [ "$2" == "noInstall" ];then
            lwarn "$pkg not installed and disable install"
            return 1    
        else
            shift 
            local enabledRepo="--enablerepo=fedora "
            for repo in $@;do
                enabledRepo="$enabledRepo --enablerepo=$repo"
            done    
            #dnf --assumeyes --disablerepo=* --enablerepo=fedora install "$1"    
            #lsudo dnf --disablerepo=* --enablerepo=fedora install "$1"    
            lsudo dnf --disablerepo=* $enabledRepo install "$pkg"    
            return $? 
        fi
    fi
}

function disableSelinux(){
    if [ `selinuxenabled` ];then 
        lsudo setenforce 0
        lsudo sed -i 's;SELINUX=enforcing;SELINUX=disabled;' /etc/selinux/config
    fi
}

function initCheck(){
    if [ -r /etc/redhat-release ];then
	case $(cat /etc/redhat-release) in
	    Fedora*):
		Vendor=fedora
		Ver=$(rpm -E %fedora)
		;;
	esac
    fi
    
    return

    local who=`whoami`
    [ "$who" == "$USER" ] || lerror "effective user:$who is not login user:$USER"
    [ -f ${HOMEDIR}.gitconfig ] || lerror " prepare .gitconfig first,exit" 
    [ -f ${HOMEDIR}.ssh/id_rsa ] || lerror "prepare ssh key first,exit"
    if [ -d $TOOLSDIR ];then
        verbose "$TOOLSDIR exist"
        #exit 0
    else
        mkdir -p $TOOLSDIR
        #cd $TOOLSDIR
    fi

    #dirGit
    local dirGit="{HOMEDIR}noteGit/git/dirGit.sh"
    if [ ! -L ${TOOLSDIR}dirGit.sh ];then
        ln -s ${dirGit} ${TOOLSDIR}dirGit.sh 
        chmod a+x ${TOOLSDIR}dirGit.sh
    fi
    local preDir="$PWD"
    cd $TOOLSDIR
    ${TOOLSDIR}dirGit.sh pull
    cd $preDir
}

#keyboard
xorgKeyboardConf=/etc/X11/xorg.conf.d/00-keyboard.conf
function initKeyBoard(){
    #swapcaps,ctrl
    pkgCheckInstall xorg-x11-xkb-utils
    #setxkbmap -option ctrl:swapcaps
    if [ -f $xorgKeyboardConf ];then
        local count=`grep -c 'ctrl:swapcaps' $xorgKeyboardConf`
        if [ $count -eq 0 ];then 
            verbose "modify $xorgKeyboardConf"
            lsudo sed -i '/EndSection/ i \\tOption "XKbOptions" "ctrl:swapcaps"' $xorgKeyboardConf
        else
            verbose "$xorgKeyboardConf had configured"
        fi
    else
        lwarn "$xorgKeyboardConf not exist"        
    fi
}


function initVim(){
    local dir=${TOOLSDIR}vim/
    pkgCheckInstall vim-X11
    pkgCheckInstall vim-enhanced
    pkgCheckInstall ctags 
    pkgCheckInstall cscope 
    if [ ! -d $dir ];then 
        git clone git@github.com:rowanpang/vim_noVundle.git $dir
    else
        verbose "$dir exist"
    fi
    [ -L ${HOMEDIR}.vim ] ||  ln -s $dir ${HOMEDIR}.vim
    lsudo [ -L ${ROOTHOME}.vim ] || lsudo ln -sf $dir ${ROOTHOME}.vim            #for root vim

    lsudo sed  -i 's; \[\s\+.*\]\s\+\&\&\s\+return$;#&;' /etc/profile.d/vim.sh
}

function initI3wm(){
    #config order
        #1. ~/.config/i3/config (or $XDG_CONFIG_HOME/i3/config if set)
        #2. /etc/xdg/i3/config (or $XDG_CONFIG_DIRS/i3/config if set)
        #3. ~/.i3/config
        #4. /etc/i3/config

    #base 
	local dir=${TOOLSDIR}i3wm/
	pkgCheckInstall i3
	pkgCheckInstall i3lock
	#pkgCheckInstall i3-doc      #about 124Mib,too big ignore
	if [ ! -d $dir ];then
	    git clone git@github.com:rowanpang/i3.git $dir
	    ln -rsf ${dir}config-v4.12 ${dir}config    
	    ln -s $dir ${HOMEDIR}.i3
	else
	    verbose "$dir exist" 
	fi

    #polkit-gnome
	pkgCheckInstall polkit-gnome
    #wallpaper
	pkgCheckInstall feh 
    #xtrlock
	pkgCheckInstall pam-devel
    #memSave
	pkgCheckInstall usermode-gtk
	pkgCheckInstall i3lock
	local memSaveDir="${dir}screenlock/memSave/"
	local pamConf="/etc/pam.d/system-auth"
	sed -i "s;PROGRAM=.*;PROGRAM=${memSaveDir}memSave.sh;" ${memSaveDir}memSave.consolehelper
	lsudo sed -i 's;auth\s\+sufficient\s\+pam_unix.so.*;& nodelay;' $pamConf                #disable pwd error delay
	lsudo ln -sf ${memSaveDir}memSave.consolehelper /etc/security/console.apps/memSave
	lsudo ln -sf ${memSaveDir}memsave.pam /etc/pam.d/memsave
	lsudo ln -rsf `which consolehelper` /usr/bin/memSave 

    #---------disable pwd quality check
	local oldUnix="$(sed --quiet '/^password\s\+\S\+\s\+pam_unix.so.*/ p' $pamConf)"
	local newUnix=$(echo "$oldUnix" | sed 's/use_authtok//')
	lsudo sed -i 's/^password\s\+\S\+\s\+pam_pwquality.so.*/#&/' $pamConf
	lsudo sed -i 's/^password\s\+\S\+\s\+pam_cracklib.*/#&/' $pamConf
	if [ "$oldUnix" != "$newUnix" ];then
	    lsudo sed -i 's/^password\s\+\S\+\s\+pam_unix.so.*/#&/' $pamConf
	    lsudo sed -i "/#password\s\+\S\+\s\+pam_unix.so.*/a$newUnix" $pamConf
	fi
    #end---------disable pwd quality check

    #terminal for nautilus
	pkgCheckInstall gnome-terminal-nautilus

    #volume
	pkgCheckInstall volumeicon
	pkgCheckInstall pavucontrol
	#sed -i "/^#volumeicon/ aexec /usr/bin/volumeicon" ${dir}config

    #nm-applet
	pkgCheckInstall network-manager-applet
	#sed -i "/^#nm-applet/ aexec /usr/bin/nm-applet" ${dir}config

    #terminator
	pkgCheckInstall terminator
	mkdir -p ${HOMEDIR}.config/terminator
	ln -sf ${dir}dep/terminator/config ${HOMEDIR}.config/terminator/config 

    #xfce4-terminator
	local confDir=${HOMEDIR}.config/xfce4/terminal
	pkgCheckInstall xfce4-terminal-0.6.3
	mkdir -p $confDir
	ln -sf ${dir}dep/xfce4-terminal/* $confDir
}

function initNutstore(){
    #depend
    pkgCheckInstall java-*-openjdk
    pkgCheckInstall notify-python

    local pkgInfo=`rpm -qi nautilus-nutstore`    
    if ! [ ${#pkgInfo} -gt 30 ];then
        verbose "installing nutstore"
        lsudo rpm -i https://www.jianguoyun.com/static/exe/installer/fedora/nautilus_nutstore_amd64.rpm
    else
        verbose "nutstore installed"
    fi
}


function initSynergy(){
    local dir=${TOOLSDIR}synergy/
    if [ ! -d $dir ];then
        git clone git@github.com:rowanpang/synergy.git $dir
        lsudo ln -s ${dir}synergy.conf /etc/synergy.conf
    else
        verbose "$dir exist" 
    fi
}

#called by initToolsMisc,shouldn't directly call
function toolsMisc_kvm(){
    [ $1 ] || lerror "init kvm need dir param"
    local dir=$1
    pkgCheckInstall virt-manager
    [ $? ] && lsudo usermod --append --groups libvirt $USER
    pkgCheckInstall libvirt-client

    local kvmDir=${HOMEDIR}vm-iso/
    [ -d $kvmDir ] || mkdir -p $kvmDir
    ln -rsf ${dir}kvm/fw24.xml ${dir}kvm/template.xml
    ln -sf ${dir}kvm/vmStart.sh ${kvmDir}vmStart.sh
    ln -sf ${dir}kvm/isoMK.sh ${kvmDir}isoMK.sh
    ln -sf ${dir}kvm/vmUsb.sh ${kvmDir}vmUsb.sh

    pkgCheckInstall bridge-utils
    pkgCheckInstall NetworkManager

    local bridgeName="bridge0"
    local bridgeConName=$bridgeName
    local slave=$(ip link | grep '^[1-9]\+: en*' | awk 'BEGIN {FS=":"} { print $2}')
    local slaveConName="${bridgeName}-slave-${slave}"

    local qemuConfig="/etc/qemu/bridge.conf"
    if [ $(cat $qemuConfig | grep -c $bridgeName) -lt 1 ];then
	lsudo sed -i "$ iallow bridge0" $qemuConfig
    fi

    if ! [ $(brctl show | grep -c $bridgeName) -gt 0 ];then
        verbose "add bridge con $bridgeConName"
        nmcli connection add ifname $bridgeName con-name $bridgeConName type bridge
        nmcli connection up $bridgeConName
    fi
    if ! [ $(brctl show | grep $bridgeName | grep -c $slave) -gt 0 ];then
        verbose "add slave $slave to $bridgeName and make connection $slaveConName"
        nmcli connection add ifname ${slave} con-name $slaveConName type  bridge-slave master $bridgeName
        nmcli connection up $slaveConName
    fi
}

#called by initToolsMisc,shouldn't directly call
function toolsMisc_diskMount(){
    [ $1 ] || lerror "init diskMount need dir param"
    local dir=$1
    local uRulesDir="/etc/udev/rules.d/"
    local etcSymdDir="/etc/systemd/system/"
    lsudo cp ${dir}diskMount/99-udisk.rules ${uRulesDir}99-udisk.rules
    lsudo cp ${dir}diskMount/auto*.service $etcSymdDir
    lsudo sed -i "s;^ExecStart=.*;ExecStart=${dir}diskMount/udev_disk_auto_mount.sh %I add;" ${etcSymdDir}autoMount@.service
    lsudo sed -i "s;^ExecStart=.*;ExecStart=${dir}diskMount/udev_disk_auto_mount.sh %I remove;" ${etcSymdDir}autoUmount@.service

    local selfSymdUdevd="${etcSymdDir}systemd-udevd.service"
    lsudo cp /usr/lib/systemd/system/systemd-udevd.service ${selfSymdUdevd}
    lsudo sed -i 's;^MountFlags;#&;' ${selfSymdUdevd}
}

function toolsMisc_httpShare(){
    [ $1 ] || lerror "init diskMount need dir param"
    local dir=$1
    lsudo ln -sf ${dir}httpShare/httpShare.sh /usr/bin/httpShare.sh
}

function initToolsMisc(){
    local dir=${TOOLSDIR}toolsMisc/
    if [ ! -d $dir ];then
        git clone git@github.com:rowanpang/toolsMisc.git $dir
    else
        verbose "$dir exist" 
    fi
    #kvm
    toolsMisc_kvm $dir
    #diskMount
    toolsMisc_diskMount $dir
    #httpShare
    toolsMisc_httpShare $dir
}

function initXXnet(){
    local dir=${TOOLSDIR}xx-net/
    pkgCheckInstall pyOpenSSL
    if [ ! -d $dir ];then
        git clone git@github.com:rowanpang/XX-Net.git $dir
    else
        verbose "$dir exist" 
    fi

    lsudo cp ${dir}code/default/xx_net.sh /etc/init.d/xx_net
    lsudo sed -i "/^PACKAGE_VER_FILE=/ iPACKAGE_PATH=\"${dir}code/\"" /etc/init.d/xx_net
    lsudo chkconfig --add xx_net
}

function initWine(){
    pkgCheckInstall samba-winbind-clients updates 
    pkgCheckInstall wine updates
}

function initWireshark(){
    pkgCheckInstall wireshark-gtk
    pkgCheckInstall wireshark-qt
    [ $? ] && lsudo usermod --append --groups wireshark,usbmon $USER
}

function initGNU(){
    pkgCheckInstall gcc-c++	 #will auto dependence gcc e.g
}

function initRepo(){
    if [ $Vendor == "fedora" ];then
	lsudo dnf --assumeyes install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm
    fi

    lsudo sed -i "s/^metadate_expire=.*/#&/" /etc/yum.repos.d/*
    lsudo sed -i "$ a metadate_expire=never" /etc/dnf/dnf.conf
}

function initSoft(){
    pkgCheckInstall mplayer rpmfusion-free rpmfusion-free-updates
    pkgCheckInstall kernel-devel
    pkgCheckInstall mediainfo
    pkgCheckInstall nmap
}

function main(){
    callFunc initCheck
    callFunc initRepo
    callFunc initGNU
    callFunc disableSelinux
    callFunc initVim
    callFunc initKeyBoard
    callFunc initNutstore
    callFunc initSynergy
    callFunc initI3wm
    callFunc initToolsMisc
    callFunc initXXnet
    callFunc initWireshark
    callFunc initSoft
}

#main

DEBUG=''
HOMEDIR="/home/$USER/"
ROOTHOME="/root/"
TOOLSDIR="${HOMEDIR}tools/"

[ $1 ] &&  DEBUG='yes'
main
