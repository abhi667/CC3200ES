# invoke SourceDir generated makefile for subscribe_publish_sample.pem4
subscribe_publish_sample.pem4: .libraries,subscribe_publish_sample.pem4
.libraries,subscribe_publish_sample.pem4: package/cfg/subscribe_publish_sample_pem4.xdl
	$(MAKE) -f C:\AWS_CC3200ES3_Publish_Subscribe/src/makefile.libs

clean::
	$(MAKE) -f C:\AWS_CC3200ES3_Publish_Subscribe/src/makefile.libs clean

