/*
 * Bond several ethernet interfaces into a Cisco, running 'Etherchannel'.
 *
 *
 * Portions are (c) Copyright 1995 Simon "Guru Aleph-Null" Janes
 * NCM: Network and Communications Management, Inc.
 *
 * BUT, I'm the one who modified it for ethernet, so:
 * (c) Copyright 1999, Thomas Davis, tadavis@lbl.gov
 *
 *	This software may be used and distributed according to the terms
 *	of the GNU Public License, incorporated herein by reference.
 *
 * 2003/03/18 - Amir Noam <amir.noam at intel dot com>
 *	- Added support for getting slave's speed and duplex via ethtool.
 *	  Needed for 802.3ad and other future modes.
 *
 * 2003/03/18 - Tsippy Mendelson <tsippy.mendelson at intel dot com> and
 *		Shmulik Hen <shmulik.hen at intel dot com>
 *	- Enable support of modes that need to use the unique mac address of
 *	  each slave.
 *
 * 2003/03/18 - Tsippy Mendelson <tsippy.mendelson at intel dot com> and
 *		Amir Noam <amir.noam at intel dot com>
 *	- Moved driver's private data types to bonding.h
 *
 * 2003/03/18 - Amir Noam <amir.noam at intel dot com>,
 *		Tsippy Mendelson <tsippy.mendelson at intel dot com> and
 *		Shmulik Hen <shmulik.hen at intel dot com>
 *	- Added support for IEEE 802.3ad Dynamic link aggregation mode.
 *
 * 2003/05/01 - Amir Noam <amir.noam at intel dot com>
 *	- Added ABI version control to restore compatibility between
 *	  new/old ifenslave and new/old bonding.
 *
 * 2003/12/01 - Shmulik Hen <shmulik.hen at intel dot com>
 *	- Code cleanup and style changes
 *
 * 2005/05/05 - Jason Gabler <jygabler at lbl dot gov>
 *      - added definitions for various XOR hashing policies
 */

#ifndef _LINUX_IF_BONDING_H
#define _LINUX_IF_BONDING_H

#include <linux/if.h>
#include <linux/types.h>
#include <linux/if_ether.h>

/* userland - kernel ABI version (2003/05/08) */
#define BOND_ABI_VERSION 2

/*
 * We can remove these ioctl definitions in 2.5.  People should use the
 * SIOC*** versions of them instead
 */
#define BOND_ENSLAVE_OLD		(SIOCDEVPRIVATE)
#define BOND_RELEASE_OLD		(SIOCDEVPRIVATE + 1)
#define BOND_SETHWADDR_OLD		(SIOCDEVPRIVATE + 2)
#define BOND_MUC_DUMP_ROUTES		(SIOCDEVPRIVATE + 3)
#define BOND_BMC_CHANNEL		(SIOCDEVPRIVATE + 4) 	/* Add/Remove a new channel to the broadcast list */
#define BOND_MUIC_CHANNEL		(SIOCDEVPRIVATE + 5) 	/* Add/Remove a new channel to the slave */
#define BOND_MUIC_ROUTE			(SIOCDEVPRIVATE + 6) 	/* Route add/del/update */
#define BOND_MUIC_BROADCAST_CHANNEL	(SIOCDEVPRIVATE + 7) 	/* Broadcast channel add/del/update */
#define BOND_MUIC_SWITCH_CHANNEL	(SIOCDEVPRIVATE + 8) 	/* Explicitly switch channel for a slave */
#define BOND_MUIC_STATS			(SIOCDEVPRIVATE + 9)	/* Get stats about channel usage from BOND_MUIC_STATS */
#define BOND_MUIC_CHANGE_CHAN_TIME	(SIOCDEVPRIVATE + 10)   /* Change channel minimum / maximum time for all bond slaves */

#define BOND_SLAVE_INFO_QUERY_OLD	(SIOCDEVPRIVATE + 11)
#define BOND_INFO_QUERY_OLD		(SIOCDEVPRIVATE + 12)
#define BOND_CHANGE_ACTIVE_OLD		(SIOCDEVPRIVATE + 13)
#define BOND_MUC_ROUTE			(SIOCDEVPRIVATE + 14) 	/* Route add/del/update */

#define BOND_CHECK_MII_STATUS	(SIOCGMIIPHY)

#define BOND_MODE_ROUNDROBIN	0
#define BOND_MODE_ACTIVEBACKUP	1
#define BOND_MODE_XOR			2
#define BOND_MODE_BROADCAST		3
#define BOND_MODE_8023AD        4
#define BOND_MODE_TLB           5
#define BOND_MODE_ALB			6 /* TLB + RLB (receive load balancing) */
#define BOND_MODE_MUC		7 /* Multi Interface CCK */
#define BOND_MODE_BMC		8 /* Multi Channel Broadcast CCK */
#define BOND_MODE_MUIC		9 /* Multi Interface + Multi Channel CCK */

/* each slave's link has 4 states */
#define BOND_LINK_UP    0           /* link is up and running */
#define BOND_LINK_FAIL  1           /* link has just gone down */
#define BOND_LINK_DOWN  2           /* link has been down for too long time */
#define BOND_LINK_BACK  3           /* link is going back */

/* each slave has several states */
#define BOND_STATE_ACTIVE       0   /* link is active */
#define BOND_STATE_BACKUP       1   /* link is backup */

#define BOND_DEFAULT_MAX_BONDS  2   /* orig value is 1.Default maximum number of devices to support */

#define BOND_DEFAULT_TX_QUEUES 16   /* Default number of tx queues per device */

#define BOND_DEFAULT_RESEND_IGMP	1 /* Default number of IGMP membership reports */

/* hashing types */
#define BOND_XMIT_POLICY_LAYER2		0 /* layer 2 (MAC only), default */
#define BOND_XMIT_POLICY_LAYER34	1 /* layer 3+4 (IP ^ (TCP || UDP)) */
#define BOND_XMIT_POLICY_LAYER23	2 /* layer 2+3 (IP ^ MAC) */
#define BOND_XMIT_POLICY_MUIC	3 /* layer 2+3 (IP ^ MAC) */

typedef struct ifbond {
	__s32 bond_mode;
	__s32 num_slaves;
	__s32 miimon;
} ifbond;

typedef struct ifslave {
	__s32 slave_id; /* Used as an IN param to the BOND_SLAVE_INFO_QUERY ioctl */
	char slave_name[IFNAMSIZ];
	__s8 link;
	__s8 state;
	__u32  link_failure_count;
} ifslave;

struct ad_info {
	__u16 aggregator_id;
	__u16 ports;
	__u16 actor_key;
	__u16 partner_key;
	__u8 partner_system[ETH_ALEN];
};

#endif /* _LINUX_IF_BONDING_H */

/*
 * Local variables:
 *  version-control: t
 *  kept-new-versions: 5
 *  c-indent-level: 8
 *  c-basic-offset: 8
 *  tab-width: 8
 * End:
 */

