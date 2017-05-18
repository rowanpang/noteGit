#!/bin/sh
#arg1:build or unbuild or none
#arg2:Ops num,
#    when build do [1,num)
#    when unbuild do (vthMax-num,vthmax] ,vthmax=7 for vth7

usage(){
    echo "./mwanInit.sh"
    echo "./mwanInit.sh unbuild"

    echo "./mwanInit.sh build 2 or ./mwanInit.sh 2"
    echo "./mwanInit.sh unbuild 2"
}

verbose(){
    if [ "$DEBUG" == 'yes' ];then 
        echo $@
    fi
}

lwarn(){
    #33m,yellow
    echo -e "\033[1;33m" "$@" "\033[0m"
}

lerror(){
    #31m,red
    echo -e "\033[1;31m" "$@" "\033[0m"
    exit -1
}

pkgCheckInstall(){
    local pkg=$1
    count=`opkg list-installed "$pkg" | grep -c "$pkg"`
    if [ $count -gt 0 ];then
        verbose "$pkg has been installed,return"
        return 0    
    else
	if [ "$2" == "noInstall" ];then
            lwarn "$pkg not installed and disable install"
            return 1
        else
	    opkg install "$pkg"
            return $? 
        fi
    fi
}

macTmp="zero e0:05:c5:2f:84:29 d2:7b:1f:29:4c:90 ee:76:20:14:c9:7e de:c1:16:9d:a4:12 8a:46:a9:00:79:98 aa:78:a3:36:58:c0 
	36:98:03:22:c8:ed 96:15:f6:06:89:7b 9e:9c:b3:d7:0e:44 be:a5:57:90:ef:b2 1e:e8:35:d1:e4:e4"

linkCheckBuild(){
    local vlink=$1
    local master=$2
    local vlinkIndex=${vlink#${vlinkPrefix}}
    shift 2
    
    local initScript="/etc/rc.local"
    local doWhat=$1   #args obvious specify by forEach...
    verbose "target link \"$vlink\" do $doWhat"

    if [ "$doWhat" == "unbuild" ];then
    	ifconfig $vlink down 2>/dev/null
    	ip link delete link $master $vlink type macvlan 2>/dev/null
	
	let local lStart=$(sed -n "/vlinkStart/=" $initScript)+1
	local lEnd=$(sed -n "/vlinkEnd/=" $initScript)
	if [ $lStart -eq $lEnd ];then
	    return
       	fi
 	let local lRange=$lEnd-$lStart-1
	sed -i "$lStart,$lRange d" $initScript

    else
    	local cmd="ip link add link $master $vlink type macvlan"
	local i=0;
	local maclink;
	for mac in $macTmp;do
		[ $i -eq $vlinkIndex ] && maclink=$mac
		let i+=1
	done
    	local cmd2="ifconfig $vlink hw ether $maclink"
    	local cmd3="ifconfig $vlink up"

	if [ $(ip link | grep -c "${vlink}@${master}") -lt 1 ];then
	    verbose "do cmd: \"$cmd\""
	    $cmd
      	    verbose "do cmd: \"$cmd2\""
	    $cmd2
      	    verbose "do cmd: \"$cmd3\""
	    $cmd3
	fi

	if [ $(cat $initScript | grep -c "$vlink") -lt 1 ];then
	    verbose "write \"$cmd\" to $initScript"
	    verbose "write \"$cmd2\" to $initScript"
 	    sed -i "/^#vlinkEnd/ i${cmd}"  $initScript
 	    sed -i "/^#vlinkEnd/ i${cmd2}" $initScript
 	    sed -i "/^#vlinkEnd/ i${cmd3}" $initScript
	    sed -i "/^#vlinkEnd/ i" $initScript
        fi
    fi
}

ifCheckBuildPost(){
    if [ "$(uci changes network)" ];then
	verbose "ifCheckBuildPost network commit and reload"
	uci commit network
	/etc/init.d/network reload
    fi 

    if [ "$(uci changes firewall)" ];then
	verbose "ifCheckBuildPost firewall commit and reload"
	uci commit firewall 
	/etc/init.d/firewall reload
    fi
}

ifCheckBuild(){
    local vlink="$1"
    local master="$2"
    shift 2

    local doWhat=$1   #args obvious specify by forEach...
    verbose "targt link \"$vlink\" do $doWhat"

    local vlinkIndex=${vlink#${vlinkPrefix}}
    local ifName="${vifPrefix}${vlinkIndex}"
    local fzone="wan"
    local fzi=0
    local ret=0
    local found="no"
    while [ $ret -eq 0 ];do
	zoneName=$(uci get "firewall.@zone[$fzi].name" 2>/dev/nulll)
   	ret=$?
	if [ $ret ] && [ "$zoneName" == "$fzone" ];then
	    #verbose "found firewall zone $fzone index: $fzi"
	    found="yes"
	    break;
 	fi
	let fzi+=1
    done	

    if [ "$found" == "no" ];then
	lerror "not found zone wan"
	exit
    fi

    local cfg="firewall.@zone[$fzi].network"
    local curMember=$(uci get $cfg)
    local masterIf="wan"

    if [ "$doWhat" == "unbuild" ];then
	uci del network.$ifName 2>/dev/null

	if [ $(echo "$curMember" | grep -c $ifName) -gt 0 ];then
	    verbose "del $ifName from firewall zone $fzone"
	    curMember=$(echo "$curMember" | sed "s# *$ifName##")
	    curMember=${curMember#\'}
	    curMember=${curMember%\'}
	    uci delete $cfg
	    uci set "$cfg=${curMember}"
	fi	
    else
	local hostName=$(uci get system.@system[0].hostname)-${vlinkIndex}
    	local masterMetric=$(uci get network.${masterIf}.metric)
    	let metric=$masterMetric+${vlinkIndex}

    	if ! [ $(uci get network.$ifName 2>/dev/null) ];then
	    verbose "build network.$ifName ,metric: $metric"

	    uci set "network.$ifName=interface"
            uci set "network.${ifName}.proto=dhcp"
            uci set "network.${ifName}.ifname=$vlink"
            uci set "network.${ifName}.metric=$metric"
	    #uci set "network.${ifName}.hostname=$hostName"
    	fi
	
	if [ $(echo "$curMember" | grep -c $ifName) -lt 1 ];then
	    verbose "add $ifName to firewall zone $fzone"
	    uci delete $cfg
	    curMember=${curMember#\'}
	    curMember=${curMember%\'}
	    uci set "$cfg=${curMember} $ifName"
	fi
    fi 
}

mwan3ConfigPost(){
    if [ "$(uci changes mwan3)" ];then
	verbose "mwan3ConfigPost commit and restart"
	uci commit mwan3
	mwan3 restart
    fi
}

mwan3Config(){
    local vlink="$1"
    local master="$2"
    shift 2
    
    local doWhat=$1   #args obvious specify by forEach...
    verbose "targt link \"$vlink\" do $doWhat"
    local vlinkIndex=${vlink#${vlinkPrefix}}
    local ifName="${vifPrefix}${vlinkIndex}"
    local memName="mem_${ifName}"
    local policyName="single_$ifName"	

    if [ "$doWhat" == "unbuild" ];then
	#mwan3 interface
	uci del mwan3.$ifName 2>/dev/null
	
	#mwan3 member
	uci del mwan3.$memName 2>/dev/null
	
	#mwan3 policy
	uci del mwan3.$policyName 2>/dev/null
	uci del_list mwan3.balanced.use_member=$memName	#add member to balanced policy
    else
	#mwan3 interface
    	if ! [ $(uci get mwan3.$ifName 2>/dev/null) ];then
	    verbose "build mwan3.$ifName"
    	    uci set mwan3.$ifName=interface
	    uci set mwan3.$ifName.count=1
    	    uci set mwan3.$ifName.down=3   
    	    uci set mwan3.$ifName.enabled=1   
    	    uci set mwan3.$ifName.interval=5   
    	    uci set mwan3.$ifName.reliability=1   
    	    uci set mwan3.$ifName.timeout=5   
    	    uci add_list mwan3.$ifName.track_ip=61.135.169.125  
    	    uci add_list mwan3.$ifName.track_ip=112.25.60.135
    	    uci set mwan3.$ifName.up=3   
      	fi
	
	#mwan3 member
	if ! [ $(uci get mwan3.$memName 2>/dev/null) ];then
	    verbose "build mwan3.$memName"
	    uci set mwan3.$memName=member
	    uci set mwan3.$memName.interface=$ifName
	    uci set mwan3.$memName.metric=1
	    uci set mwan3.$memName.weight=1
	fi

	#mwan3 single vwan policy  
	if ! [ $(uci get mwan3.$policyName 2>/dev/null) ];then
	    verbose "build mwan3.$policyName"
	    uci set mwan3.$policyName=policy
	    uci set mwan3.$policyName.last_resort=default
	    uci add_list mwan3.$policyName.use_member=$memName
	fi
	
	#balanced policy
	if ! [ $(uci get mwan3.balanced.use_member | grep -c "$memName") -gt 0 ];then
	    verbose "build add to mwan3.balanced.use_member"
	    uci add_list mwan3.balanced.use_member=$memName	#add member to balanced policy
	fi	
    fi 
}

#arg0: func 
#arg1: build/unbuild
forEachvlink(){
    [ $# -gt 1 ] || lerror "no func OR no cmd args"
    local master="eth0.2"
    local vthIndexStart=1                                                            
    local vthOpsNum=8
    local doFunc="$1" && shift 
    local doWhat="$1" 
    local doFuncPost="${doFunc}Post"

    [ $opsNum -ne 255 ] && vthOpsNum=$opsNum

    if [ "$doWhat" == "build" ];then
    	let local vthMax=$vthOpsNum+$vthIndexStart                                                   
    	local i=$vthIndexStart                                                              
    else
 	let local vthMax=$(ip link | grep -c "$vlinkPrefix[0-9]\+@$master")+1
    	[ $opsNum -ne 255 ] || vthOpsNum=$vthMax
	[ $vthOpsNum -ge $vthMax ] && let vthOpsNum=$vthMax-1
	let local i=$vthMax-$vthOpsNum
    fi	

    verbose "vlink [$i,$vthMax) call func: \"$doFunc\" do: \"$doWhat\""

    while [ $i -lt $vthMax ];do                                                         
        local vthName="${vlinkPrefix}$i"                                                           
        let i+=1                                                                        
        $doFunc $vthName $master $@
    done       

    if [ "X$(type -t ${doFuncPost})" != "X" ];then
	$doFuncPost
    fi
}
 
initMwan(){
    pkgCheckInstall kmod-macvlan
    pkgCheckInstall mwan3
    pkgCheckInstall luci-app-mwan3
    pkgCheckInstall ip

    echo    
    local cmd="build" 
    [ "$1" == "unbuild" ] && cmd="unbuild" 
    if [ $cmd == "build" ];then
	forEachvlink linkCheckBuild $cmd
	echo    
	forEachvlink ifCheckBuild $cmd
	echo    
	forEachvlink mwan3Config $cmd
    else
	forEachvlink mwan3Config $cmd
	echo    
	forEachvlink ifCheckBuild $cmd
	echo    
	forEachvlink linkCheckBuild $cmd
    fi
}

#main
DEBUG='yes'
vlinkPrefix="vth"
vifPrefix="vwan"
opsNum=255
[ "$1" == "unbuild" ] && { doWhat="unbuild";shift; }
[ "$1" ] && opsNum=$1

initMwan $doWhat
