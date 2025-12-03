#
# Regular cron jobs for the chaos-crew-dca package.
#
0 4	* * *	root	[ -x /usr/bin/chaos-crew-dca_maintenance ] && /usr/bin/chaos-crew-dca_maintenance
