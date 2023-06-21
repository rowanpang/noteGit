#!/bin/bash
function promptconfirm()
{
    local prompt="$1: "
    local uinput=""
    local uconfirm=""

    while :;do
        read -p "$prompt" uinput
        prompt="User input: \"$uinput\" ,please confirm[YyNn](y): "
        while : ;do
            read -p "$prompt"  uconfirm
            case $uconfirm in
                Y | y | N | n | "")
                    break
                    ;;
                * )
                    prompt=$'\t'"Input invalid,please reconfirm[YyNn](y): "
                    ;;
            esac
        done

        if [ "X$uconfirm" == "Xn" ] || [ "X$uconfirm" == "XN" ];then
            prompt="$1: "
        else
            break
        fi
    done

    echo "$uinput"
}

function promptYn()
{
    local prompt="$1,Please Confirm[YyNn](y): "
    local uconfirm=""
    local ret

    while :;do
        read -p "$prompt"  uconfirm
        case $uconfirm in
            Y | y | "")
                ret='Y'
                break
                ;;
            N | n )
                ret='N'
                break
                ;;
            * )
                prompt=$'\t'"Input invalid,please reconfirm[YyNn](y): "
                ;;
        esac
    done
    echo "$ret"
}

echo `promptYn "running, kill or not"`
echo `promptconfirm "kkkkk"`
echo
LD_SHOW_AUXV=1 sleep 1

