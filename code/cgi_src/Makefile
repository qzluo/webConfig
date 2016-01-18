CC = g++
ARMCC = arm-linux-g++

start:
	$(CC) readTermioParas.c -o readTermioParas.cgi
	$(CC) writeTermioParas.c cgi_common.c -o writeTermioParas.cgi
	$(CC) readDataTrans.c -o readDataTrans.cgi
	$(CC) clearDataTrans.c -o clearDataTrans.cgi
	$(CC) appendDataTrans.c cgi_common.c -o appendDataTrans.cgi
	$(CC) restart.c -o restart.cgi
	$(CC) setNetwork.c -o setNetwork.cgi
	$(CC) startService.c -o startService.cgi
	$(CC) stopService.c -o stopService.cgi
	$(CC) uploadcgi.c -o uploadcgi.cgi

arm-compile:
	$(ARMCC) readTermioParas.c -o readTermioParas.cgi
	$(ARMCC) writeTermioParas.c cgi_common.c -o writeTermioParas.cgi
	$(ARMCC) readDataTrans.c -o readDataTrans.cgi
	$(ARMCC) clearDataTrans.c -o clearDataTrans.cgi
	$(ARMCC) appendDataTrans.c cgi_common.c -o appendDataTrans.cgi
	$(ARMCC) restart.c -o restart.cgi
	$(ARMCC) setNetwork.c -o setNetwork.cgi
	$(ARMCC) startService.c -o startService.cgi
	$(ARMCC) stopService.c -o stopService.cgi
	$(ARMCC) uploadcgi.c -o uploadcgi.cgi

install:
	cp *.cgi /home/lbh/share/www/cgi-bin/

clean:
	rm -fr *.cgi
