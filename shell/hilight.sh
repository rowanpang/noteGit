#/bin/bash
virsh -c qemu:///system net-list --all | grep --color -E '^|active'
