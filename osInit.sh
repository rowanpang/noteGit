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
			return 0
		fi
	fi
}

HOMEDIR="/home/$USER/"
ROOTHOME="/root/"
TOOLSDIR="${HOMEDIR}tools/"
function initCheck(){
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
	setxkbmap -option ctrl:swapcaps
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
	[ -L ${ROOTHOME}.vim ] || lsudo ln -sf $dir ${ROOTHOME}.vim			#for root vim

	lsudo sed  -i 's; \[\s\+\"`.*\];#&;' /etc/profile.d/vim.sh
}

function initI3wm(){
	#config order
		#1. ~/.config/i3/config (or $XDG_CONFIG_HOME/i3/config if set)
        #2. /etc/xdg/i3/config (or $XDG_CONFIG_DIRS/i3/config if set)
        #3. ~/.i3/config
        #4. /etc/i3/config

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
	local memSaveDir="${dir}screenlock/memSave/"
	sed -i "s;PROGRAM=.*;PROGRAM=${memSaveDir}memSave.sh;" ${memSaveDir}memSave.consolehelper
	lsudo sed -i 's;auth\s\+sufficient\s\+pam_unix.so.*;& nodelay;' /etc/pam.d/system-auth
	lsudo ln -sf ${memSaveDir}memSave.consolehelper /etc/security/console.apps/memSave
	lsudo ln -sf ${memSaveDir}memsave.pam /etc/pam.d/memsave
	lsudo ln -rsf `which consolehelper` /usr/bin/memSave 
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

function initToolsMisc(){
	local dir=${TOOLSDIR}toolsMisc/
	if [ ! -d $dir ];then
		git clone git@github.com:rowanpang/toolsMisc.git $dir
	else
		verbose "$dir exist" 
	fi
	
	#kvm
	local kvmDir=${HOMEDIR}vm-iso/
	[ -d $kvmDir ] || mkdir -p $kvmDir
	ln -rsf ${dir}kvm/fw24.xml ${dir}kvm/template.xml
	ln -sf ${dir}kvm/vmStart.sh ${kvmDir}vmStart.sh
	ln -sf ${dir}kvm/isoMK.sh ${kvmDir}isoMK.sh

	#diskMount
	local uRulesDir="/etc/udev/rules.d/"
	local udevdService="/etc/systemd/system/systemd-udevd.service"
	lsudo ln -sf ${dir}diskMount/99-udisk.rules ${uRulesDir}99-udisk.rules
	lsudo cp /usr/lib/systemd/system/systemd-udevd.service ${udevdService}
	lsudo sed -i 's;^MountFlags;#&;' ${udevdService}
}

function initXXnet(){
	local dir=${TOOLSDIR}xx-net/
	if [ ! -d $dir ];then
		git clone git@github.com:rowanpang/XX-Net.git $dir
		lsudo ln -s ${dir}code/default/xx_net.sh /etc/init.d/xx_net
		lsudo chkconfig --add xx_net
	else
		verbose "$dir exist" 
	fi

	pkgCheckInstall pyOpenSSL
}

function initWine(){
	pkgCheckInstall samba-winbind-clients updates 
	pkgCheckInstall wine updates
}

function main(){
	callFunc initCheck
	callFunc initVim
	callFunc initKeyBoard
	callFunc initNutstore
	callFunc initSynergy
	callFunc initI3wm
	callFunc initToolsMisc
	callFunc initXXnet
}

#main
DEBUG=''
[ $1 ] &&  DEBUG='yes'
main
