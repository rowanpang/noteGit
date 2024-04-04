0,basic
	a,while1 kthread test

1,summary
	a,环境
		Linux localhost.localdomain 3.10.0-1160.99.1.el7.ppc64le #1 SMP Tue Sep 12 15:13:13 UTC 2023 ppc64le ppc64le ppc64le GNU/Linux
		
		# CONFIG_PREEMPT_RCU is not set
		CONFIG_PREEMPT_NOTIFIERS=y
		# CONFIG_PREEMPT_NONE is not set
		CONFIG_PREEMPT_VOLUNTARY=y
		# CONFIG_PREEMPT is not set
			
	b,目的
		验证kthread在不主动让出cpu时,cpu是否可以调度其它task执行.
	c,结论
		在!PREEMPT kernel上, 如果不主动让出(sleep),则不会调度其它task执行;
	d,原因
		tick 可以check并标记kthread为need resched,
		但kthread 运行在内核态, !PREEMPT 禁止内核抢占,在该cpu上没有调度时机.

	e,遇到问题
		inmod 正常, rmmode时服务器停止响应, 但仍然可以ping通
		Ref hung.txt
		
2,步骤
	a,sysctl
	  sysctl kernel.softlockup_panic=1

	b,insmod
		...
		[18518.123036] delay 500ms:    49
		[18518.623038] delay 500ms:    50
		[18519.123039] delay 500ms:    51
		[18519.623041] delay 500ms:    52
		[18520.021827] NMI watchdog: BUG: soft lockup - CPU#3 stuck for 22s! [pmount_kthread:12100]
		[18520.021839] Modules linked in: while1(OE) binfmt_misc ip6t_rpfilter ipt_REJECT nf_reject_ipv4 ip6t_REJECT nf_reject_ipv6 xt_conntrack ip_set ebtable_nat ebtable_broute bridge stp llc ip6table_nat nf_conntrack_ipv6 nf_defrag_ipv6 nf_nat_ipv6 ip6table_mangle ip6table_security ip6table_raw iptable_nat nf_conntrack_ipv4 nf_defrag_ipv4 nf_nat_ipv4 nf_nat nf_conntrack iptable_mangle iptable_security iptable_raw ebtable_filter ebtables ip6table_filter ip6_tables iptable_filter sg virtio_balloon ip_tables xfs libcrc32c sr_mod cdrom virtio_blk virtio_scsi virtio_net net_failover failover bochs_drm drm_kms_helper syscopyarea sysfillrect sysimgblt fb_sys_fops ttm drm drm_panel_orientation_quirks i2c_core virtio_pci virtio_ring virtio dm_mirror dm_region_hash dm_log dm_mod
		[18520.021871] CPU: 3 PID: 12100 Comm: pmount_kthread Kdump: loaded Tainted: G           OE  ------------   3.10.0-1160.99.1.el7.ppc64le #1
		[18520.021873] task: c0000003ec94c600 ti: c0000003f0afc000 task.ti: c0000003f0afc000
		[18520.021874] NIP: c000000000026ec0 LR: d000000007d600fc CTR: c000000000026e80
		[18520.021876] REGS: c0000003f0affa90 TRAP: 0901   Tainted: G           OE  ------------    (3.10.0-1160.99.1.el7.ppc64le)
		[18520.021877] MSR: 8000000000009033 <SF,EE,ME,IR,DR,RI,LE>  CR: 28000024  XER: 20000000
		[18520.021881] CFAR: c000000000026ed0 SOFTE: 1
		GPR00: d000000007d600ec c0000003f0affd10 c00000000148c600 000000000f424000
		GPR04: c000000001aca820 c000000001add8e8 202020203a736d30 0000000000000000
		GPR08: 0000000000000007 000000000c2b78ba 00000c7372b9b162 d000000007d602b0
		GPR12: c000000000026e80 c000000007b41c80
		[18520.021891] NIP [c000000000026ec0] udelay+0x40/0x60
		[18520.021893] LR [d000000007d600fc] threadfn+0xfc/0x104 [while1]
		[18520.021894] Call Trace:
		[18520.021895] [c0000003f0affd10] [d000000007d600ec] threadfn+0xec/0x104 [while1] (unreliable)
		...
	c,softlockup 说明没有发生调度;
