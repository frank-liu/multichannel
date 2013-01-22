/*
 * signal_quality.c
 *
 *  Created on: 22 Jan 2013
 *      Author: Frank LIU
 *      Email: frank.kingdom@gmail.com
 */

#include "signal_quality.h"

/*------------------------------Signal Level-------------------------------------------*/
/* From:
 * http://www.linuxquestions.org/questions/linux-wireless-networking-41/ioctl-call-for-obtaining-the-rssi-611337/
 *	Return 0 for success
 */
int signal_level(char *if_name)
{
	int sockfd;
	struct iw_statistics stats;
	struct iwreq req;
	memset(&stats, 0, sizeof(stats));
	memset(&req, 0, sizeof(iwreq));
	sprintf(req.ifr_name, if_name);
	req.u.data.pointer = &stats;
	req.u.data.length = sizeof(iw_statistics);
#ifdef CLEAR_UPDATED
	req.u.data.flags = 1;
#endif

	/* Any old socket will do, and a UDP (datagram) socket is pretty cheap */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Could not create simple datagram socket");
		exit(EXIT_FAILURE);
	}

	/* Perform the ioctl */
	if (ioctl(sockfd, SIOCGIWSTATS, &req) == -1)
	{
		perror("Error performing SIOCGIWSTATS");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	close(sockfd);

	printf("Signal level%s is %d%s.\n",
			(stats.qual.updated & IW_QUAL_DBM ? " (in dBm)" : ""),
			stats.qual.level,
			(stats.qual.updated & IW_QUAL_LEVEL_UPDATED ? " (updated)" : ""));

	return 0;
}
