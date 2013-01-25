/*
 * signal_quality.c
 *
 *  Created on: 22 Jan 2013
 *      Author: Frank LIU
 *      Email: frank.kingdom@gmail.com
 */

//#include "signal_quality.h"
#include <linux/wireless.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <sys/ioctl.h>

//#include </usr/include/stdlib.h>

/*------------------------------Signal Level-------------------------------------------*/
/* From:
 * http://www.linuxquestions.org/questions/linux-wireless-networking-41/ioctl-call-for-obtaining-the-rssi-611337/
 *	Return 0 for success
 */
int signal_level(char *if_name)
{
	//int sockfd;
	int fd = ioctl_fd();
	struct iw_statistics stats;
	struct iwreq iwreq;
	memset(&stats, 0, sizeof(stats));
	memset(&iwreq, 0, sizeof(iwreq));
	sprintf(iwreq.ifr_name, "rausb0");
	iwreq.u.data.pointer = &stats;
	iwreq.u.data.length = sizeof(stats);
	#ifdef CLEAR_UPDATED
	iwreq.u.data.flags = 1;
	#endif

	/* Any old socket will do, and a UDP (datagram) socket is pretty cheap
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Could not create simple datagram socket");
		exit(EXIT_FAILURE);
	}*/

	/* Perform the ioctl */
		if (fd >= 0 && ioctl (fd, SIOCGIWSTATS, &iwreq) >= 0)//if (ioctl(sockfd, SIOCGIWSTATS, &req) == -1)
	{
		perror("Error performing SIOCGIWSTATS");
		close(fd);
		exit(-1);
	}

	close(fd);

	printf("Signal level%s is %d%s.\n",
			(stats.qual.updated & IW_QUAL_DBM ? " (in dBm)" : ""),
			stats.qual.level,
			(stats.qual.updated & IW_QUAL_LEVEL_UPDATED ? " (updated)" : ""));

	return 0;
}
